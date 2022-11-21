### Lightweight Subnet Calculator
*© Mike Rotella 2022, GNU GPL_v3*

Calculate network address, broadcast address, and host
range for a given IP and mask. Mask can be in slash
notation or dotted decimal. Provides error checking and
error feedback.

<code>    test\
    test  
    test  
</code>

**Build:**<br>
<code>gcc -o ip subnet.c</code><br>

**Examples:**<br>
<code>% ./ip 10.248.17.99/27<br>
    IP Address: 10.248.17.99\
    Mask: 255.255.255.224
    Network address is: 10.248.17.96
    Broadcast address is: 10.248.17.127
    IP range is: 10.248.17.97 - 10.248.17.126
</code>

<code>% ./ip 172.16.24.0 255.192.0.0
    IP Address: 172.16.24.0
    Mask: 255.192.0.0
    Supernet IP range is: 172.0.0.0 - 172.63.255.255
</code>

<code>% ./ip 233.40.20.9/29
    IP Address: 233.40.20.9
    Mask: 255.255.255.248
    Multicast range: 233.40.20.8/29
    Range is: 233.40.20.8 - 233.40.20.15
</code>