# 🐚 CSEShell — Custom Shell Environment

CSEShell is a lightweight and user-friendly shell built in C that supports useful built-in commands, additional system utilities, and enhancements to improve user experience.

---

# How to compile and run our shell
```bash
make
./cseshell
```

Note:
```bash
make clean
``` 
each time you edit the code

---

## 🔧 Built-in Commands Supported

### 🛠 Shell-specific Commands

| Command    | Description                                                                 |
|------------|-----------------------------------------------------------------------------|
| `cd`       | Change the current working directory of the shell. The current directory is shown in the prompt. |
| `help`     | Print a list of all builtin commands available in CSEShell.                 |
| `usage`    | Print a brief description of how to use each builtin command. Also provides a helpful message when an unknown command is entered. |
| `env`      | Print all current environment variables, including inherited system variables. |
| `setenv`   | Add a new environment variable using `setenv KEY=VALUE`. Silent on success. Use `env` to verify. |
| `unsetenv` | Remove an environment variable using `unset KEY`. No effect if the variable doesn't exist. |

---

### ⚙️ System Utilities

| Command    | Description                                                                 |
|------------|-----------------------------------------------------------------------------|
| `sys`      | Display system information (similar to `neofetch`) including user, OS, and terminal info. |
| `dspawn`   | Spawn a background daemon process.                                          |
| `dcheck`   | Show the number of currently running daemon processes.                      |
| `backup`   | Zip the directory specified by `$BACKUP_DIR` and move the archive to `[PROJECT_DIR]/archive/`. |
| `find`     | Search for files in a specified directory.                                  |
| `ld`       | List the contents of the current directory.                                 |
| `ldr`      | Recursively list contents of the current directory and its subdirectories.  |

---

## ✨ Additional Features

- 🧭 **Decorated Prompt**: The prompt includes the current user, path, and time—styled in a cyberpunk theme using neon pink, purple, and cyan.
- 🌟 **Personal Touch**: A cute symbol `⋆.ೃ࿔*:･` follows the username for added flair!
- 🎨 **Randomized Input Colors**: Every user input appears in a random color to add visual variety.
- 💬 **Style Toggle for Prompt**: Users can switch the prompt text style between:
  - `style dim` → for privacy or reduced visual intensity  
  - `style normal` → default view  
  - `style bold` → improved visibility for users with visual impairments

---

## 🖼 Shell Preview

<img width="1205" height="490" alt="Screenshot 2025-07-30 224404" src="https://github.com/user-attachments/assets/5aee768d-1382-422f-9ed1-f828906b1a1c" />

---

## 🌱 Sustainability and Inclusivity

### 🌱 Sustainability

- 🧠 To promote energy-conscious computing, CSEShell shows **CPU runtime, memory usage, and disk I/O** after each command.
- ⚙️ This is implemented using `getrusage()`, which tracks system resource usage in both user and kernel space.
- 📉 By exposing these metrics, users are empowered to monitor the impact of their commands and make informed decisions—especially valuable for low-power or battery-constrained devices.

---

### 🌍 Inclusivity

- 💡 While changing font size was not feasible in a terminal environment, CSEShell supports **text style customization** for the prompt:
  - `style dim` → ideal for private, low-contrast environments  
  - `style normal` → clean, standard appearance  
  - `style bold` → improves legibility for users with visual impairments
- 🗣️ These settings help cater to diverse visual preferences and enhance accessibility for all users.

---

