#!/usr/bin/env sh

set -eu

MALCOLM_NS="malcolm"
TARGET_NS="target"
MALCOLM_IF="veth-malcolm"
TARGET_IF="veth-target"
MALCOLM_IP="10.13.37.10/24"
TARGET_IP="10.13.37.20/24"

need_root()
{
	if [ "$(id -u)" -ne 0 ]; then
		printf '%s\n' "Run this script with sudo."
		exit 1
	fi
}

clean()
{
	ip netns del "$MALCOLM_NS" 2>/dev/null || true
	ip netns del "$TARGET_NS" 2>/dev/null || true
}

setup()
{
	clean

	ip netns add "$MALCOLM_NS"
	ip netns add "$TARGET_NS"

	ip link add "$MALCOLM_IF" type veth peer name "$TARGET_IF"
	ip link set "$MALCOLM_IF" netns "$MALCOLM_NS"
	ip link set "$TARGET_IF" netns "$TARGET_NS"

	ip netns exec "$MALCOLM_NS" ip addr add "$MALCOLM_IP" dev "$MALCOLM_IF"
	ip netns exec "$TARGET_NS" ip addr add "$TARGET_IP" dev "$TARGET_IF"

	ip netns exec "$MALCOLM_NS" ip link set lo up
	ip netns exec "$TARGET_NS" ip link set lo up
	ip netns exec "$MALCOLM_NS" ip link set "$MALCOLM_IF" up
	ip netns exec "$TARGET_NS" ip link set "$TARGET_IF" up

	printf '%s\n' "Namespaces ready."
	status
}

status()
{
	printf '\n%s\n' "malcolm namespace:"
	ip netns exec "$MALCOLM_NS" ip -brief addr show "$MALCOLM_IF"
	printf '  MAC: '
	ip netns exec "$MALCOLM_NS" cat "/sys/class/net/$MALCOLM_IF/address"

	printf '\n%s\n' "target namespace:"
	ip netns exec "$TARGET_NS" ip -brief addr show "$TARGET_IF"
	printf '  MAC: '
	ip netns exec "$TARGET_NS" cat "/sys/class/net/$TARGET_IF/address"

	printf '\n%s\n' "Use the target MAC as the fourth ft_malcolm argument."
}

usage()
{
	printf '%s\n' "Usage: sudo $0 setup|status|clean"
}

need_root
case "${1:-}" in
	setup)
		setup
		;;
	status)
		status
		;;
	clean)
		clean
		;;
	*)
		usage
		exit 1
		;;
esac
