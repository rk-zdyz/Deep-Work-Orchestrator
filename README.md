# 🧠 Deep Work Orchestrator

A lightweight, zero-dependency Windows CLI tool that enforces deep focus sessions by **blocking distracting websites**, **launching your workspace apps**, and **continuously killing time-wasting processes** — all from a single executable that requires no installation.

Built with pure C++ and the Win32 API.

---

## ✨ Features

| Feature | How It Works |
|---|---|
| 📄 **Unified Config** | All settings in a single `config.ini` file with INI-style sections and comment support. |
| 🌐 **Website Sinkholing** | Redirects distracting domains to `127.0.0.1` via the system `hosts` file. Automatically backs up and restores on exit. |
| 🚀 **Workspace Launcher** | Opens your productivity apps (VS Code, terminals, browsers, etc.) at session start via `ShellExecute`. |
| 💀 **Distraction Killer** | Polls running processes every 5 seconds and terminates any that match your blocklist (case-insensitive matching). |
| 🛡️ **Graceful Exit** | Intercepts `Ctrl+C` to cleanly restore network rules and end the session. |
| 🔒 **Runs as Admin** | Embedded manifest auto-triggers a UAC elevation prompt — required for `hosts` file access and process termination. |

---

## 📁 Project Structure

```
Deep-Work-Orchestrator/
├── include/
│   ├── ConfigLoader.h           # Unified config parser declarations
│   ├── EnvironmentLauncher.h    # Workspace launcher declarations
│   ├── NetworkSinkHole.h        # Website blocker declarations
│   └── ProcessManager.h         # Process killer declarations
├── src/
│   ├── main.cpp                 # Entry point and orchestration loop
│   ├── ConfigLoader.cpp         # INI-style config file parser
│   ├── EnvironmentLauncher.cpp  # ShellExecute-based app launcher
│   ├── NetworkSinkHole.cpp      # Hosts file sinkholing & restore
│   └── ProcessManager.cpp       # Snapshot-based process termination
├── config.ini                   # Unified configuration file
├── app.manifest                 # UAC elevation manifest (requireAdministrator)
├── build.bat                    # One-click MSVC build script
└── README.md
```

---

## 🛠️ Prerequisites

- **Windows 10/11**
- **Visual Studio Build Tools 2022** (or any edition with the MSVC C++ toolchain)
  - Download: [aka.ms/vs/17/release/vs_BuildTools.exe](https://aka.ms/vs/17/release/vs_BuildTools.exe)
  - Install the **"Desktop development with C++"** workload

---

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/rk-zdyz/Deep-Work-Orchestrator.git
cd Deep-Work-Orchestrator
```

### 2. Configure Your Session

Edit `config.ini` in the project root. All settings live in one file with INI-style sections:

```ini
# Lines starting with '#' are comments

[websites]
# Domains to redirect to 127.0.0.1 (www. variant is added automatically)
youtube.com
reddit.com
twitter.com
instagram.com

[applications]
# Programs to launch when the session starts (must be in PATH or use full path)
code
alacritty

[distractions]
# Process names to auto-kill (case-insensitive, use .exe name from Task Manager)
discord.exe
steam.exe
spotify.exe
```

> 💡 **Tip:** You don't need to rebuild after editing `config.ini` — just restart `orchestrator.exe`.

### 3. Build

Simply double-click `build.bat` or run it from any terminal:

```bash
build.bat
```

> The build script auto-detects your VS Build Tools installation — no Developer Command Prompt required.

### 4. Run

```bash
orchestrator.exe
```

> 🛡️ Windows will show a **UAC prompt** — click **Yes**. Administrator access is required to modify the hosts file and terminate other processes.

---

## 🔄 Session Lifecycle

```
┌─────────────────────────────────────────────┐
│          orchestrator.exe launched           │
│              (Admin privileges)              │
└──────────────────┬──────────────────────────┘
                   │
          ┌────────▼────────┐
          │  Load config.ini│  ← [websites], [applications], [distractions]
          └────────┬────────┘
                   │
          ┌────────▼────────┐
          │  Block Websites │  → hosts file sinkholing
          └────────┬────────┘
                   │
         ┌─────────▼─────────┐
         │  Launch Workspace  │  → ShellExecute
         └─────────┬─────────┘
                   │
        ┌──────────▼──────────┐
        │   Monitor & Kill    │  → every 5s (case-insensitive)
        │   (active loop)     │
        └──────────┬──────────┘
                   │  Ctrl+C
         ┌─────────▼─────────┐
         │  Restore Network  │  → hosts file backup restored + DNS flush
         └─────────┬─────────┘
                   │
            ┌──────▼──────┐
            │  Session End │
            └─────────────┘
```

---

## ⚠️ Important Notes

- **Run as Administrator** — The tool modifies `C:\Windows\System32\drivers\etc\hosts`. Without admin rights, website blocking will fail silently.
- **Hosts file backup** — A backup is created at `hosts.backup` before modifications. It is restored automatically on `Ctrl+C`.
- **DNS Cache** — The tool automatically flushes your DNS cache on session end. If sites are still blocked after exit, run manually:
  ```bash
  ipconfig /flushdns
  ```
- **Process matching** — Process names are matched **case-insensitively**, so `discord.exe` will match `Discord.exe`.
- **Multi-process apps** — Apps like Discord (Electron-based) spawn multiple processes. The orchestrator kills all instances and will re-kill them if the app auto-restarts.
- **Antivirus** — Some antivirus software may flag process termination behavior. You may need to whitelist `orchestrator.exe`.

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).
