# ft_malcolm

A lightweight ARP spoofing tool written in C that intercepts ARP requests and sends spoofed ARP replies to poison a target's ARP cache.

## Build

```bash
make
```

## Usage

Root privileges are required for raw socket access (`AF_PACKET`).

```bash
sudo ./ft_malcolm <source_ip> <source_mac> <target_ip> <target_mac>
```

| Parameter | Description |
|---|---|
| `<source_ip>` | Source IP address to spoof |
| `<source_mac>` | MAC address to bind to the source IP |
| `<target_ip>` | Target machine's IP address |
| `<target_mac>` | Target machine's MAC address |

### Example

```bash
sudo ./ft_malcolm 10.13.37.10 aa:bb:cc:dd:ee:ff 10.13.37.20 02:00:00:00:00:02
```

## Testing

A network namespace script is included to test in an isolated environment:

```bash
# Set up test namespaces (malcolm & target)
sudo ./scripts/netns-test.sh setup

# Clean up namespaces
sudo ./scripts/netns-test.sh clean
```

Verify the ARP cache on the target:

```bash
arp -n
# or
ip neigh show
```
