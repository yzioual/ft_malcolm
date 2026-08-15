ft_malcolm: Introduction to ARP Spoofing & MITM
1. Overview & Theoretical Background

The Address Resolution Protocol (ARP), defined in RFC 826, is a fundamental Layer 2 (Data Link Layer) protocol responsible for mapping Internet Protocol (IPv4) network addresses to physical Media Access Control (MAC) addresses on a local area network (LAN).
The Inherent Vulnerability of ARP

ARP was designed without authentication or state verification:

    Statelessness: Operating systems process and store incoming ARP replies even if the host never initiated a corresponding ARP request (often referred to as unsolicited or gratuitous ARP handling).

    Lack of Authentication: ARP packets contain no cryptographic signatures, headers, or validation mechanisms to verify the true identity of the sender.

    Broadcast Trust Model: ARP requests are broadcast to every device on the local network segment (FF:FF:FF:FF:FF:FF), allowing any host on the collision domain to inspect and respond to queries.

2. The Man-in-the-Middle (MITM) Mechanism

In a classic local network environment, when Host A (Target) attempts to transmit an IP packet to Host B (Gateway or Source), it executes the following resolution cycle:

[Host A (Target)]                                 [Host B (Source)]
       |                                                 |
       | ------ Broadcast ARP Request ("Who has IP B?") ->|
       |                                                 |
       |<------ Unicast ARP Reply ("IP B is at MAC B") ---|
       |                                                 |

The ARP Poisoning Flow in ft_malcolm

ft_malcolm operates by intercepting the legitimate query and injecting a forged identity into the transaction:

[Target: 192.168.1.10]                     [Attacker: ft_malcolm]
       |                                              |
       | ---- Broadcast: "Who has 10.0.2.99?" ------->| (Intercepted)
       |                                              |
       |<---- Unicast Reply: "10.0.2.99 is at MAC X" -| (Spoofed Reply)
       |                                              |
[Target Cache Updated: 10.0.2.99 -> MAC X]

    Target Broadcast: The victim needs to deliver traffic to a given source IP. Having no cached entry, it broadcasts an ARPOP_REQUEST (0x0001).

    Attacker Interception: ft_malcolm captures raw frames directly at Layer 2 using an AF_PACKET socket bound to the active network interface.

    Filter & Match: Incoming frames are checked against the command-line parameters (verifying sender IP/MAC and target requested IP).

    Forged Response: ft_malcolm immediately constructs an ARPOP_REPLY (0x0002) assigning the requested source IP to the designated spoofed MAC address.

    Cache Poisoning: The victim machine receives the reply and binds the source IP to the spoofed MAC in its local neighbor/ARP table.

3. Project Architecture & Implementation Details

The program operates across three primary phases: initialization, packet interception, and frame crafting.
Frame Layout Structure

Each constructed packet encapsulates both the Ethernet Layer 2 header and the ARP payload:

+-------------------------------------------------------------------+
|                        Ethernet Header (14B)                      |
|  Dest MAC (6B)  |  Source MAC (6B)  |  EtherType: 0x0806 (2B)     |
+-------------------------------------------------------------------+
|                          ARP Header (28B)                         |
|  Hardware Type: 0x0001 (Ethernet)   |  Protocol Type: 0x0800 (IP) |
|  HW Size: 6     |  Proto Size: 4    |  Opcode: 0x0002 (Reply)     |
|  Sender MAC (6B - Spoofed)          |  Sender IP (4B - Spoofed)   |
|  Target MAC (6B - Victim)           |  Target IP (4B - Victim)    |
+-------------------------------------------------------------------+

Key Components

    Interface Resolution (getifaddrs): Dynamically enumerates system network interfaces, selecting the active, running non-loopback device.

    Raw Socket Initialization (socket, bind): Instantiates an AF_PACKET socket with SOCK_RAW and ETH_P_ARP filter to intercept Link-Layer Ethernet frames without kernel TCP/IP stack interference.

    Validation & Parsing (inet_pton, custom MAC parser): Validates strict dot-decimal IPv4 notations and colon-separated hexadecimal MAC addresses passed via CLI.

    Signal Handling (sigaction / signal): Catches SIGINT (Ctrl+C) to terminate cleanly and close open file descriptors.

4. Usage & Execution
Bash

# Compilation
make

# Syntax
sudo ./ft_malcolm <source_ip> <source_mac> <target_ip> <target_mac>

# Example Execution
sudo ./ft_malcolm 10.13.37.1 aa:bb:cc:dd:ee:ff 10.13.37.2 02:00:00:00:00:02

Verification

Check the updated entry on the target machine:
Bash

ip neigh show
# or
arp -n

5. Defenses & Mitigations

Several mitigation strategies exist at the network infrastructure and host levels:

    Dynamic ARP Inspection (DAI): Switch-level security feature that validates ARP packets against a trusted DHCP snooping database.

    Static ARP Entries: Manually configuring permanent MAC-to-IP mappings on critical infrastructure (e.g., default gateways).

    ARP Spoofing Detection Tools: Host-based daemons (e.g., arpwatch) that monitor network traffic and alert administrators when MAC-to-IP associations change unexpectedly.

    Network Segmentation & 802.1X: Restricting local broadcast domains using VLANs and port authentication to limit attack surfaces.
