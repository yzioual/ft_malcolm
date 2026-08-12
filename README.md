# ft_malcolm | 42

---

## An introduction to Man in the Middle attacks. This is the first project of a network security branch at 42.


### What's ARP?
- Address Resolution Protocol, and is used to match IP address to its physical MAC address on a local network.

### How does ARP work?
- A device sends a boradcast message to the whole network asking 'who has this IP address?'. That's an ARP request.
- The machine with that IP address sends a direct personal (unicast) response back with its MAC address. That's an ARP reply.

### Man In the Middel attack?
- ARP Spoofing/Poisoning: Hackers send fake replies to link their own computer's MAC address to a legitimate IP address (like the gateway router), letting them intercept or block local data traffic.
