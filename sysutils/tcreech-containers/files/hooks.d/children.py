#!/usr/local/bin/python3
import sys
import json
import subprocess
import os
from datetime import datetime
from pathlib import Path

# Configuration
LOG_FILE = "/tmp/oci_children.log"

def log(message):
    try:
        with open(LOG_FILE, "a") as f:
            f.write(f"{datetime.now()}: {message}\n")
    except Exception:
        pass

def run_cmd(args):
    try:
        subprocess.run(args, check=True, capture_output=True)
        return True
    except subprocess.CalledProcessError as e:
        log(f"Command failed: {' '.join(args)} - Error: {e.stderr.decode().strip()}")
        return False

def main():
    try:
        input_data = sys.stdin.read()
        if not input_data:
            sys.exit(0)
        data = json.loads(input_data)
        container_id = data.get("id")
        status = data.get("status")
        children_max = data.get("annotations", {}).get("org.freebsd.jail.children.max")

        if not container_id or not status or not children_max:
            sys.exit(1)

        # Validate the value
        if not int(children_max):
            log(f"Requested children.max={children_max} is not an integer")
            sys.exit(1)
        children_max = int(children_max)
        if not children_max > 0:
            log(f"Requested children.max={children_max} is not positive")
            sys.exit(1)

        if status == "created":
            log(f"Creating {container_id}, setting children.max={children_max}")
            # Create Jail
            run_cmd([
                "/usr/sbin/jail", "-vm", f"name={container_id}", 
                f"children.max={children_max}"
            ])

    except Exception as e:
        log(f"Unexpected error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
