#!/usr/local/bin/python3
import sys
import json
import subprocess
import os
from datetime import datetime
from pathlib import Path

# Configuration
LOG_FILE = "/tmp/oci_linprocfs.log"

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
    log("running")
    try:
        input_data = sys.stdin.read()
        if not input_data:
            sys.exit(0)
        data = json.loads(input_data)
        container_id = data.get("id")
        status = data.get("status")
        can_mount_linprocfs = data.get("annotations", {}).get("org.freebsd.jail.allow.mount.linprocfs")

        if not container_id or not status or not can_mount_linprocfs:
            sys.exit(1)

        can_mount_linprocfs = json.loads(can_mount_linprocfs)
        log(f"got can_mount_linprocfs={can_mount_linprocfs}")
        log(f"bool? {isinstance(can_mount_linprocfs, bool)}")

        # Validate the value
        if not can_mount_linprocfs:
            log(f"Requested allow.mount.linprocfs={can_mount_linprocfs} is not truthy")
            sys.exit(1)

        if status == "created":
            log(f"Creating {container_id}, setting allow.mount.linprocfs=true")
            # Create Jail
            run_cmd([
                "/usr/sbin/jail", "-vm", f"name={container_id}", 
                f"allow.mount.linprocfs=true"
            ])

    except Exception as e:
        log(f"Unexpected error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
