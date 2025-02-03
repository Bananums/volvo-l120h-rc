# rc-volvo-l120h


## Pseudo-Terminal Device, socat, for UART simulation

Create a Virtual UART Pair

```shell
socat -d -d pty,raw,echo=0 pty,raw,echo=0

```