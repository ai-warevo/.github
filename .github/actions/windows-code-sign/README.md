# windows-code-sign

Composite action to import a code signing certificate into Windows certificate stores. Prepares the Windows build environment for signed executables and installers.

## Purpose

When building Windows applications that need to be code signed (EXE, MSI, DLL), the signing certificate must be:
1. Decoded from base64 (PFX format, stored as GitHub secret)
2. Imported to `CurrentUser\My` store (accessible to signers like `signtool`)
3. Optionally imported to `Root` and `TrustedPublisher` stores to suppress trust prompts
4. Cleaned up after the build

This action automates the entire process.

## Usage

Basic usage in a Windows build job:

```yaml
- name: Setup MSVC environment
  uses: ilammy/msvc-dev-cmd@v1

- name: Import code signing certificate
  uses: ai-warevo/.github/.github/actions/windows-code-sign@main
  with:
    certificate_secret: ${{ secrets.SIGNING_CERTIFICATE }}
    certificate_password: ${{ secrets.CERTIFICATE_PASSWORD }}

- name: Build and sign executable
  run: |
    # Build your application
    cmake -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build --config Release
    
    # Sign the executable (certificate is now in cert store)
    # signtool sign /sha1 <THUMBPRINT> /t http://timestamp.digicert.com build/app.exe
```

## Inputs

| Input | Required | Default | Description |
|-------|----------|---------|-------------|
| `certificate_secret` | Yes | — | Base64-encoded PFX certificate file. Usually from `secrets.SIGNING_CERTIFICATE`. |
| `certificate_password` | Yes | — | Password to unlock the PFX file. Usually from `secrets.CERTIFICATE_PASSWORD`. |
| `certificate_name` | No | `code_signing.pfx` | Temporary filename for the PFX (doesn't affect functionality). |
| `dry_run` | No | `false` | If `true`, validate base64 decoding but skip importing to certificate stores. Useful for testing. |

## Outputs

| Output | Description |
|--------|-------------|
| `certificate_imported` | `true` if certificate was successfully imported, `false` if skipped or error occurred. |
| `certificate_subject` | X.509 subject name of the imported certificate (e.g., `CN=Company, O=Organization`). Empty if not imported. |
| `certificate_path` | Temporary filesystem path where the PFX file was created (file is cleaned up after action completes). |

## Certificate Stores

The action imports into three Windows certificate stores:

| Store | Location | Purpose |
|-------|----------|---------|
| `CurrentUser\My` | Per-user personal certificates | Accessible to signers and cert queries |
| `Root` (LocalMachine) | Trusted root CAs (all users) | Prevents "untrusted root" warnings |
| `TrustedPublisher` (LocalMachine) | Trusted publishers (all users) | Suppresses SmartScreen and UAC prompts |

## Certificate Format

The `SIGNING_CERTIFICATE` secret must be:
- **Format:** PFX (PKCS#12) containing private key
- **Encoding:** Base64 (no line breaks)
- **Storage:** GitHub secret or environment variable

To create the base64 value from an existing PFX:

```bash
# macOS / Linux
base64 -i my-cert.pfx | tr -d '\n' | pbcopy

# Windows PowerShell
[Convert]::ToBase64String([IO.File]::ReadAllBytes("my-cert.pfx")) | Set-Clipboard
```

## Error Handling

The action will fail (exit code 1) if:
- `certificate_secret` is empty or malformed base64
- `certificate_password` is incorrect or certificate is corrupted
- `dry_run` is false but certificate import fails

If a step fails, all output variables are set to empty/false before exiting.

## Cleanup

The temporary PFX file is **always** cleaned up after the action completes — even if import fails — using PowerShell's `finally`-equivalent (`if: always()`).

## Example: Code-Signed Windows Build

```yaml
jobs:
  build-windows-signed:
    runs-on: windows-latest
    steps:
      - uses: actions/checkout@v4
      
      - name: Setup MSVC
        uses: ilammy/msvc-dev-cmd@v1
      
      - name: Import signing certificate
        uses: ai-warevo/.github/.github/actions/windows-code-sign@main
        with:
          certificate_secret: ${{ secrets.SIGNING_CERTIFICATE }}
          certificate_password: ${{ secrets.CERTIFICATE_PASSWORD }}
      
      - name: Configure & Build
        uses: ai-warevo/.github/.github/actions/cmake-build@main
        with:
          build_type: Release
      
      - name: Sign executable
        shell: powershell
        run: |
          # Find certificate by subject (requires signtool)
          $cert = Get-ChildItem -Path "Cert:\CurrentUser\My" | Where-Object {
            $_.Subject -like "*MyCompany*"
          } | Select-Object -First 1
          
          if ($cert) {
            Write-Host "Signing with certificate: $($cert.Subject)"
            signtool sign /sha1 $cert.Thumbprint /t http://timestamp.digicert.com `
              build/Release/myapp.exe
          } else {
            Write-Host "ERROR: Signing certificate not found in cert store"
            exit 1
          }
      
      - name: Upload signed artifact
        uses: actions/upload-artifact@v4
        with:
          name: myapp-signed
          path: build/Release/myapp.exe
```

## Testing (dry_run mode)

To test the action without actually importing certificates:

```yaml
- name: Test certificate import (dry run)
  uses: ai-warevo/.github/.github/actions/windows-code-sign@main
  with:
    certificate_secret: ${{ secrets.SIGNING_CERTIFICATE }}
    certificate_password: ${{ secrets.CERTIFICATE_PASSWORD }}
    dry_run: 'true'
```

Output will show:
- ✓ Base64 decoding succeeded (or error if malformed)
- ✓ Certificate metadata extracted
- Skips actual import to certificate stores

## See Also

- [Windows SignTool documentation](https://learn.microsoft.com/en-us/dotnet/framework/tools/signtool-exe)
- [X509 Certificate Stores](https://learn.microsoft.com/en-us/windows-hardware/drivers/install/certificate-stores)
- gitsync `release.yml` — Example consumer of this action
