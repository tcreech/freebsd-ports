--- README.md
+++ README.md
@@ -153,12 +153,34 @@
 the interface has `-ifdisabled` cleared first, since FreeBSD leaves IPv6 off
 on an interface until something asks for it and would otherwise refuse the
 address silently.
+
+## DHCP
+
+`"ipam": {"type": "dhcp"}` takes addresses from the LAN's DHCP server instead
+of a `host-local` pool. The jail has no `/dev/bpf` to broadcast from and no
+DHCP client of its own, so the host leases on the container's behalf: a
+`dhclient` runs on the epair's host-side end, identifies itself to the server
+with the container's MAC as its `dhcp-client-identifier` (so a reservation
+keyed on that MAC still resolves), and — the part that matters — *stays
+running* after the initial binding to renew the lease for the container's
+lifetime. A lease acquired and abandoned at `ADD` simply expires, and the
+container is left on an address the server has handed back out.
 
+The client's hook script configures the container side with `ifconfig -j` and
+`route -j`, so a renewal that changes the address moves the container with it.
+`DEL` stops the client; the lease itself is left to expire -- the base
+`dhclient` has no release code path at all, and none is invented here. A
+container that dies without `DEL` still cleans up, because destroying the
+jail destroys the epair and the client quits when its interface vanishes.
+DHCP here is IPv4-only; a second family needs `host-local` ranges or
+`CNI_ARGS` alongside.
+
 ## Runtime state
 
 | path | purpose |
 |---|---|
 | `/var/run/cni/freebsd-epair` | container id → epair name, so `DEL` can find it |
+| `/var/run/cni/freebsd-epair/<id>.dhcp` | per-container DHCP client state (pid, lease, hook script), `ipam: dhcp` only |
 | `/var/log/cni-freebsd-epair.log` | all logging; stderr is redirected here so it cannot corrupt the JSON podman parses |
 
 These keep their original `freebsd-epair` spelling even though the plugin is
