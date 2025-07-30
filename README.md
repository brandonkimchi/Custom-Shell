# 🐚 CSEShell — Custom Shell Environment

CSEShell is a lightweight and user-friendly shell built in C that supports useful built-in commands, additional system utilities, and quality-of-life enhancements to improve user experience.

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

- 🧭 Displays current user, date, time, and current working directory in the shell prompt.
- 📊 Displays resource usage statistics (CPU time, memory usage, and disk I/O) after each external command.
- 🧼 Simple and readable interface.
- 🧠 Designed to resemble a full-featured yet minimal shell environment.
- 💬 Includes visual feedback and informative messages for each command.

---

## 🖼 Shell Preview

> _Add a screenshot or GIF of CSEShell in action here if available._

---

## 🌱 Sustainability and Inclusivity

### ✅ Sustainability: Lightweight System Programs

CSEShell emphasizes resource efficiency by implementing lightweight versions of common system tools:

- The `sys` command is a minimal alternative to heavier tools like `neofetch`, designed to quickly display essential system information without unnecessary overhead.
- All custom utilities (e.g., `usage`, `dcheck`, `backup`) are built to execute quickly and with minimal memory and CPU usage.
- CSEShell displays real-time resource usage statistics (such as CPU time, memory consumption, and disk I/O) after running external commands. This empowers users to make informed decisions about how their commands affect system performance and supports mindful computing.

> This promotes longer battery life on laptops and better performance on resource-constrained systems, aligning with green computing principles.

---

### 🌍 Inclusivity: Clear and Friendly User Experience

CSEShell was developed with inclusivity in mind through:

- 💬 **Inclusive Error Messaging**: All error messages use plain, user-friendly language with actionable tips (e.g., "Command not found. Type `help` to see available commands.").
- 📖 **Beginner Support**: The `help` and `usage` commands offer accessible documentation for all built-in commands, making the shell easy to learn for newcomers.
- 🌐 **Minimal Technical Jargon**: Output messages, command syntax, and feedback are approachable to users from diverse technical backgrounds.

> By prioritizing both performance and accessibility, CSEShell aims to provide a productive environment for all users.

---

## 🤝 Contributing

Contributions are welcome! Feel free to open issues or submit pull requests to improve the shell, add features, or fix bugs.

---

## 📄 License

MIT License © [Your Name or Team Name]
