# ✨ Template: Multi-Platform Binary Release Workflow

**Location:** `.github/workflows/release.yml`

This template shows the pattern for projects that build and release cross-platform binaries (Linux + Windows). It demonstrates:

1. **Platform-specific build jobs** (`build-linux`, `build-windows`)
2. **Artifact collection** (merge multiple artifacts into one)
3. **Reusable release workflow** (`reusable-release.yml` for version + publish)
4. **Custom release notes** (project-specific quick-start + features)

## When to Use

- ✅ You have C++ / Go / Rust projects with multi-platform binaries
- ✅ You need platform-specific build steps (containers, MSVC setup, signing)
- ✅ You publish binaries as GitHub Release artifacts
- ✅ You want custom release notes (not auto-generated)

## When NOT to Use

- ❌ Single-platform projects (use `reusable-release.yml` directly)
- ❌ Docker image releases (use `reusable-release.yml` with `docker-build-push`)
- ❌ Library/package releases (use `reusable-release.yml` with npm/cargo publish)

## Template Structure

```
Release (workflow_dispatch or tag push)
├─ build-linux     (platform-specific)
├─ build-windows   (platform-specific)
├─ collect-artifacts (optional, project-specific)
└─ release         (uses reusable-release.yml)
```

## Customization Checklist

Before copying this template:

- [ ] Replace `gitsync` with your project name
- [ ] Update build directories (e.g., `build/release` → your build dir)
- [ ] Update CMake flags (e.g., `-DGITSYNC_BUILD_TESTS=OFF` → your flags)
- [ ] Update artifact names and paths
- [ ] Customize release notes (what-included, quick-start sections)
- [ ] Update container image if needed (default: `ghcr.io/ai-warevo/ci-cpp23`)

## Key Sections to Understand

### Build Jobs (Linux & Windows)

Each build job is **project-specific** and handles:
- Checkout
- Platform setup (container for Linux, MSVC for Windows)
- Build configuration (CMake with project-specific flags)
- Binary post-processing (strip on Linux, sign on Windows)
- Artifact upload

**You own this.** Use `ai-warevo/.github/.github/actions/cmake-build` and other shared actions to reduce boilerplate.

### Code Signing on Windows

If your Windows build needs code signing (optional):

```yaml
- name: Import code signing certificate
  uses: ai-warevo/.github/.github/actions/windows-code-sign@main
  with:
    certificate_secret: ${{ secrets.SIGNING_CERTIFICATE }}
    certificate_password: ${{ secrets.CERTIFICATE_PASSWORD }}
```

Then sign the executable:

```yaml
- name: Sign executable
  shell: powershell
  run: |
    $cert = Get-ChildItem -Path "Cert:\CurrentUser\My" | Where-Object {
      $_.Subject -like "*YourCompany*"
    } | Select-Object -First 1
    signtool sign /sha1 $cert.Thumbprint /t http://timestamp.digicert.com build/app.exe
```

### Artifact Collection

Download all platform artifacts into one directory, then upload as a single artifact:

```yaml
collect-artifacts:
  needs: [build-linux, build-windows]
  steps:
    - uses: actions/download-artifact@v4
      with:
        name: myapp-linux
        path: artifacts
    
    - uses: actions/download-artifact@v4
      with:
        name: myapp-windows
        path: artifacts
    
    - uses: actions/upload-artifact@v4
      with:
        name: myapp-release-binaries
        path: artifacts/
```

**Why?** `reusable-release.yml` expects a single artifact name. Collecting simplifies the interface.

### Reusable Release Job

This calls the shared release workflow:

```yaml
release:
  needs: collect-artifacts
  uses: ai-warevo/.github/.github/workflows/reusable-release.yml@main
  with:
    version: ${{ inputs.version || 'auto' }}
    artifact_name: myapp-release-binaries
    artifact_directory: artifacts
    what-included: |
      - Multi-platform binaries (Linux, Windows)
      - Full documentation
    quick-start: |
      **Download & run:**
      - Linux: `chmod +x myapp && ./myapp`
      - Windows: `myapp.exe`
  permissions:
    contents: write
```

**Key parameters:**
- `version` — "auto" derives from CHANGELOG.md, or override with X.Y.Z
- `artifact_name` — Must match the artifact uploaded in `collect-artifacts`
- `what-included` / `quick-start` — Markdown sections added to release notes

## Triggers

This template supports:

**1. Tag Push** (automatic)
```bash
git tag v1.2.3
git push origin v1.2.3
```
→ Release is created automatically

**2. Manual Dispatch** (with version override)
```bash
gh workflow run release.yml --ref main --field version=1.2.3
# or leave blank for auto derivation
gh workflow run release.yml --ref main
```

## Full Template

See: [`gitsync/.github/workflows/release.yml`](https://github.com/ai-warevo/gitsync/blob/main/.github/workflows/release.yml)

## Real-World Example

**Project:** gitsync (Go CLI + C++ components)
- **Linux build:** CMake + ci-cpp23 container
- **Windows build:** CMake + MSVC
- **Code signing:** Optional Windows certificate import
- **Release notes:** Custom quick-start for Linux and Windows setup

## Common Mistakes

❌ **Not collecting artifacts** — Directly using multiple `artifact_name` values
→ ✅ Use single `collect-artifacts` job to merge them

❌ **Hardcoded action SHAs** — `uses: ai-warevo/.github/.github/workflows/reusable-release.yml@abc123`
→ ✅ Always use `@main`

❌ **Forgetting `needs` dependency** — Release job runs before builds finish
→ ✅ Always add `needs: collect-artifacts` to release job

❌ **Platform-specific tests in release** — Build jobs shouldn't run tests
→ ✅ Use `reusable-pr-fast.yml` for pre-PR testing; release only builds and publishes

## Next Steps

1. Copy this template to your project: `.github/workflows/release.yml`
2. Update project-specific sections (names, flags, paths)
3. Add `CHANGELOG.md` following [Keep a Changelog](https://keepachangelog.com)
4. Test with manual dispatch: `gh workflow run release.yml`
5. Push a tag to trigger automatic release

## Support

Shared workflows and actions are in `ai-warevo/.github/.github/`.
- Actions: `.github/actions/*/`
- Workflows: `.github/workflows/reusable-*.yml`
