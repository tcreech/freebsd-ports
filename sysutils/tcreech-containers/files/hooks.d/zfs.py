#!/usr/local/bin/python3
import sys
import json
import subprocess
import os
from datetime import datetime
from pathlib import Path

# Configuration
STATE_DIR = Path("/var/run/containers/zfs")
LOG_FILE = "/tmp/oci_zfs.log"

# Ensure state directory exists
try:
    STATE_DIR.mkdir(parents=True, exist_ok=True)
except Exception as e:
    # If we can't write to /var/run, fallback to /tmp for testing
    STATE_DIR = Path("/tmp/containers/zfs")
    STATE_DIR.mkdir(parents=True, exist_ok=True)

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
        dataset = data.get("annotations", {}).get("zfs.dataset")

        if not container_id or not status or not dataset:
            sys.exit(1)

        # Persist state for the poststop stage
        state_file = STATE_DIR / f"{container_id}.json"
        with open(state_file, "w") as f:
            json.dump({"dataset": dataset}, f)

        # Verify dataset exists
        if not run_cmd(["/sbin/zfs", "list", "-Ho", "name", dataset]):
            sys.exit(1)

        if status == "created":
            log(f"Creating {container_id}, jailing dataset {dataset}")
            # Create Jail
            run_cmd([
                "/usr/sbin/jail", "-vm", f"name={container_id}", 
                "allow.mount=1", "allow.mount.zfs=1", "allow.mount", 
                "enforce_statfs=1"
            ])
            # Mount dataset to jail
            run_cmd(["/sbin/zfs", "jail", container_id, dataset])
            # Execute mount -a inside jail
            run_cmd(["/usr/sbin/jexec", container_id, "/sbin/zfs", "mount", "-a"])

        elif status == "stopped":
            log(f"Stopping {container_id}, freeing dataset {dataset}")
            # List child datasets and unmount them
            result = subprocess.run(
                ["/sbin/zfs", "list", "-Ho", "name", "-r", dataset],
                capture_output=True, text=True
            )
            for ds in result.stdout.splitlines():
                ds = ds.strip()
                if ds:
                    log(f"Unmounting {ds}")
                    run_cmd(["/sbin/umount", "-f", ds])
            
            # Clean up state file
            if state_file.exists():
                state_file.unlink()

    except Exception as e:
        log(f"Unexpected error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
