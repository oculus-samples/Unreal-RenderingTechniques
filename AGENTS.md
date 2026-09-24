# Agent Instructions — Rendering Techniques (Unreal)

An Unreal Engine sample for Meta Quest collecting several VR-friendly rendering recipes into one project: cascaded shadows, color-grading LUTs, distance-field baked shadows, two portal rendering methods, and high-quality text rendering via stereo layers.

## Source-of-truth files (read these first, do not duplicate their contents in this file)

For setup, build steps, engine version, and project layout, read:

- `README.md` — official setup, per-map technique notes, and Meta-fork build command
- `RenderingTechniques.uproject` — Unreal engine association, enabled plugins, target platforms
- `Config/` — UE config; switch the default map here to control which technique launches
- `Source/RenderingTechniques/` — C++ module (includes `PortalCaptureActor` helpers)
- `Plugins/Portals/` — project-local plugin implementing the stereo-render-target portal path
- `Content/RenderingTechniques/` — maps, materials, blueprints per technique
- `LICENSE` — license terms

## Quest / Horizon-specific notes

- Requires the Meta fork of Unreal Engine built from source (`Oculus-VR/UnrealEngine`). The prebuilt Epic Launcher engine + MetaXR plugin path is NOT sufficient here — `GenerateProjectFiles.bat` must run against the source-built engine.
- Per-technique light-mobility requirements are inconsistent and easy to mix up: cascaded shadows require movable lights; distance-field baked shadows require stationary lights. Do not generalize one to the other when refactoring.
- `Plugins/Portals/` is project-local, not a marketplace plugin. Removing or relocating it breaks PortalsMap.
- Text with transparency must NOT be premultiplied with alpha for the TextRendering map's stereo-layer path to look correct.
- This repo does not ship a `.gitattributes` LFS config at the root despite the README recommending `git lfs install`. Treat the README as authoritative if you encounter missing binary assets.

# Meta Quest tooling

This is a Meta Quest / Horizon OS sample. The bespoke intro above is the source of truth for what this project is and how it's built — use it (and the files it points at) instead of restating facts from memory.

When the user asks anything about Quest device behavior, build / deploy / debug / capture flows, on-device performance, or Horizon OS APIs, reach for these tools instead of generic Unreal answers:

- **`hzdb`** — Quest-aware ADB wrapper (device list, install / launch / stop, logs, screenshots, Perfetto traces, on-device docs search). Already wired up as an MCP server via `.mcp.json`, `.vscode/mcp.json`, and `.cursor/mcp.json`. Also runnable directly: `npx -y @meta-quest/hzdb <subcommand>`.
- **Meta Quest Agentic Tools** — the full skill set, including Unreal-specific skills: <https://github.com/meta-quest/agentic-tools>. Install per your client (Claude Code: `/plugin install meta-vr@meta-quest`; Gemini CLI: `gemini extensions install https://github.com/meta-quest/agentic-tools`; Cursor / VS Code: install the **Meta Horizon** extension from the Marketplace).

A few behavior expectations:

- **Read this repo's files first.** Before answering anything project-specific, read `README.md` and whichever source-of-truth files the intro above points at. Don't restate their contents in chat — quote or link instead.
- **Use `hzdb` for device-side work.** Anything that touches an attached Quest (install, launch, logs, screenshot, capture, manifest inspection) goes through `hzdb`, not raw `adb`.
- **Check live Horizon OS docs before answering API questions.** `hzdb docs search "..."` queries the live docs; training data on Horizon OS APIs goes stale fast.
- **Don't fabricate SDK / engine versions.** If a version isn't visible in this repo's files, say so rather than guessing.
