**Note**: Nothing can be done outside a linux system.

Developed on WSL, Ubuntu, with g++-14, and requires C++23 or later to run.

# How to run
From root

```bash
make // Build project
./bin/argent <args> // Runs the program from root.
```

If a local bin is made, simply do
```bash
argent <args>
```
If this says command not found, make sure there is a build on the machine. This is the culprit most likely.

# Attaching Local Bin
**NOTE**: This make this command work from any directory, not just root.

Run this to make the folder if not already:
```bash
mkdir -p ~/.local/bin
```

Then, navigate to `~/.bashrc` and enter this line at the very bottom.

```bash
export PATH="$HOME/.local/bin:$PATH"`
```

Navigate to the ArgentDB folder, and run this command to set up the link:
```bash
ln -sf $(pwd)/bin/argent ~/.local/bin/argent
```

This should make `argent` a global command.