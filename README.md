### Lightweight Subnet Calculator
*© Mike Rotella 2022, GNU GPL_v3*

Calculate network address, broadcast address, and host
range for a given IP and mask. Mask can be in slash
notation or dotted decimal. Provides error checking and
error feedback.

**Build:**<br>
<code>gcc -o ip subnet.c</code><br>

**Examples:**<br>
<code>% ./ip 10.248.17.99/27<br>
IP Address: 10.248.17.99  

Mask: 255.255.255.224  

Network address is: 10.248.17.96<br>
Broadcast address is: 10.248.17.127<br>
IP range is: 10.248.17.97 - 10.248.17.126<br>
</code><br>

<code>% ./ip 172.16.24.0 255.192.0.0<br>
IP Address: 172.16.24.0<br>
Mask: 255.192.0.0<br>
Supernet IP range is: 172.0.0.0 - 172.63.255.255<br>
</code><br>

<code>% ./ip 233.40.20.9/29<br>
IP Address: 233.40.20.9<br>
Mask: 255.255.255.248<br>
Multicast range: 233.40.20.8/29<br>
Range is: 233.40.20.8 - 233.40.20.15<br>
</code><br>