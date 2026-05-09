# 🧠 Deep Work Orchestrator

A lightweight, zero-dependency Windows CLI tool that enforces deep focus sessions by **blocking distracting websites**, **launching your workspace apps**, and **continuously killing time-wasting processes** — all from a single executable that requires no installation.

Built with pure C++ and the Win32 API.

---

## ✨ Features

| Feature | How It Works |
|---|---|
| 🌐 **Website Sinkholing** | Redirects distracting domains to `127.0.0.1` via the system `hosts` file. Automatically backs up and restores on exit. |
| 🚀 **Workspace Launcher** | Opens your productivity apps (VS Code, terminals, browsers, etc.) at session start via `ShellExecute`. |
| 💀 **Distraction Killer** | Polls running processes every 5 seconds and terminates any that match your blocklist. |
| 🛡️ **Graceful Exit** | Intercepts `Ctrl+C` to cleanly restore network rules and end the session. |
| 🔒 **Runs as Admin** | Embedded manifest auto-triggers a UAC elevation prompt — required for `hosts` file access and process termination. |

---

## 📁 Project Structure

```
Deep-Work-Orchastrator/
├── include/
│   ├── EnvironmentLauncher.h    # Workspace launcher declarations
│   ├── NetworkSinkHole.h        # Website blocker declarations
│   └── ProcessManager.h         # Process killer declarations
├── src/
│   ├── main.cpp                 # Entry point and orchestration loop
│   ├── EnvironmentLauncher.cpp  # ShellExecute-based app launcher
│   ├── NetworkSinkHole.cpp      # Hosts file sinkholing & restore
│   └── ProcessManager.cpp       # Snapshot-based process termination
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
git clone https://github.com/rk-zdyz/Deep-Work-Orchastrator.git
cd Deep-Work-Orchastrator
```

### 2. Configure Your Lists

Create three text files in the project root (one entry per line):

**`websites.txt`** — Domains to block
```
youtube.com
reddit.com
twitter.com
instagram.com
```

**`applications.txt`** — Apps to launch at session start
```
code
notepad++
```

**`distractions.txt`** — Process names to auto-kill
```
Discord.exe
Telegram.exe
Spotify.exe
```

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
          │  Block Websites │  ← websites.txt → hosts file
          └────────┬────────┘
                   │
         ┌─────────▼─────────┐
         │  Launch Workspace  │  ← applications.txt → ShellExecute
         └─────────┬─────────┘
                   │
        ┌──────────▼──────────┐
        │   Monitor & Kill    │  ← distractions.txt → every 5s
        │   (active loop)     │
        └──────────┬──────────┘
                   │  Ctrl+C
         ┌─────────▼─────────┐
         │  Restore Network  │  ← hosts file backup restored
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
- **DNS Cache** — After the session ends, you may need to flush your DNS cache for unblocking to take effect immediately:
  ```bash
  ipconfig /flushdns
  ```
- **Antivirus** — Some antivirus software may flag process termination behavior. You may need to whitelist `orchestrator.exe`.

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).
