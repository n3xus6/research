# Linux notes

_These are just random notes for me to remember how things work. If someone reads this, be aware that the commands can cause data loss and system corruption._

[Bash](#Bash)\
[Manual pages](#Manual-pages)\
[Directories](#Directories)\
[Search](#Search)\
[Permissions](#Permissions)\
[Program output](#Program-output)\
[Pipes](#Pipes)\
[Environment variables](#Environment-variables)\
[Logging](#Logging)\
[Shutdown and restart](#Shutdown)\
[Runlevels](#Runlevels)\
[Process management](#Process-management)\
[User management](#User-management)\
[Disk partitioning](#Disk-partitioning)\
[Network analysis](#Network-analysis)\
[IP - Internet Protocol](#IP-Internet-Protocol)\
[Multiples of bytes](#Byte-Multiples)\
[Numpad](#Numpad)\
[Tools](#Tools)\
[Miscellaneous topics](#Miscellaneous)\
[References](#References)

## Bash
### Command line navigation
* `ctrl-a` Move the cursor to the line beginning.
* `ctrl-e` Move the cursor to the line end.
* `ctrl-w` Delete the word before the cursor.
* `ctrl-k` Delete everything after the cursor.

### Wildcard expansion

Print the file names that contain a space character in the name.

	$ ls *[[:space:]]*
	#or
	$ ls *\ *

### Bash prompt

The PS1 environment variable controls the format of the bash prompt.

	$ printenv PS1
	[\u@\h \W]$

Example:

	$ export PS1="[\t \u@\h \w]\$ "
	[19:39:24 user@host /mnt/hgfs]$

### Command history

* `history` Display a list with previously typed commands.
* `Ctrl-r` Enter interactive search. Press again to loop through the matches.
* `Ctrl-g` Abort the search.
* `!<number>` Run the command by the number printed in the history.
* `!!` The last command.

		$ date
		Sat May  9 18:46:47 CEST 2020
		$ !! -I
		date -I
		2020-05-09
* `!*` - the last command parameters

		$ more foo.txt bar.txt
		$ less !*

## Manual-pages

* `man [section-number] <item>` Show the first manual page found for "item".
* `man -f <item>` / `whatis <item>` List all manual pages found for "item".
* `man -k <item>` / `apropos <item>` Search man pages that contain a reference to "item".
* navigation
	* `SPACE` Scroll down.
	* `b` Scroll up.
	* `g` Jump to the first line.
	* `G` Jump to the last line.

## Directories
* `cd ~` or `cd ~user` Switch to the user's home directory.
* `mkdir -p parent/parent/child` Create parent directories if they don't exist yet.
* `ls -latr` Show all files sorted by the modification date.
* `ls -F` Append a file type indicator to the file name.
* `whereis <command>` Show the full path of command.

Some examples of standard program installation paths on Linux
* google-earth-pro
  * _/usr/bin/google-earth-pro_
* lynx
  * _/usr/local/bin/lynx_
  * _/usr/local/etc/lynx.cfg_
* openssl
  * _/usr/bin/openssl_
  * _/usr/include/openssl_
  * _/usr/share/man/man1/openssl.1ssl.gz_

## Search

Files starting with "foo" and smaller than 5 megabytes:

    $ find . -name "foo*" -type f -size -5M

Case insensitive search for directories starting with "bar":

	$ find . -iname "bar*" -type d

Pass all matches as an argument list to a subsequent command:

	$ find . -name "foo*" -type f -exec du -h '{}' ';'
	#or
	$ find . -name "foo*" -type f -print0 | xargs -0 du -h

### Index search with the locate command

	$ locate <name>

Update the database for the locate command:

	$ sudo updatedb

### Show path of command

	$ which <command>

## Permissions

### Change file mode bits

| r     | w     | x     |
| :---: | :---: | :---: |
| 4     | 2     | 1     |

	$ chmod =765 myprog
	-rwxrw-r-x. 1 user group 0 Mar 24 17:35 myprog

### setuid bit 's'

If the 's' bit is set an executable is run as the owner of the file instead of the current user. For instance, passwd has the 's' bit set so users can change their own password while passwd can save the settings in files that belong to root.

	# Display the Access Control List (ACL). Note the 's' under flags.
	$ getfacl /usr/bin/passwd
	# file: usr/bin/passwd
	# owner: root
	# group: root
	# flags: s--
	user::rwx
	group::r-x
	other::r-x

	# While passwd is running one can see that the process belongs to 'root' because of the setuid bit.
    $ ps -ef | grep passwd
    root       8152   3339  0 14:38 pts/0    00:00:00 passwd

### sticky bit 't'

If several users have access to the same directory like /tmp it would be possible to delete the files from users there. This can be disabled by setting the sticky bit on the directory:

	chmod a+t <dir>

## Program-output

Redirect stdout and stderr to separate files:

	./a.out 1> output.txt 2> error.txt

Redirect all output to a single file:

	./a.out 1> all.txt 2>&1

## Pipes
One powerful UNIX feature is to concatenate several programs with pipes '|' to perform a complex task. Example:

	$ cat population.csv
	Europa;746
	Afrika;1305
	Latainamerika;645
	Asien;4587
	Ozeanien;42
	Nordamerika;367

	$ cat population.csv | tr ';' ' ' | awk '{ print $2 " " $1 }' | sort -nr | column -t
	4587  Asien
	1305  Afrika
	746   Europa
	645   Latainamerika
	367   Nordamerika
	42    Ozeanien

## Environment-variables

* `export NAME="value"` Create or update variable.
* `unset NAME` Delete variable.

## Logging

* `dmesg` Print kernel ring buffer.
* `dmesg --level=warn` Only print kernel warnings.
* `journalctl` Display the system log.
* `logger -p <facility>.<severity> <msg>` Enter a message into the system log.

Example:

	$ logger -p local1.info hello
	$ journalctl
	...
	Mar 26 19:37:19 localhost user[10185]: hello

System log files: _/var/log/messages*_

## Shutdown

* `shutdown` Power-off in 1min.
* `shutdown now` Power-off immediately.
* `shutdown -c` Cancel a pending shutdown.
* `shutdown -r` Reboot.
* `shutdown -r +15` Reboot in 15 minutes.

## Runlevels

`systemctl isolate <target>` Change to a runlevel.

	$ ll /usr/lib/systemd/system/
	/usr/lib/systemd/system/runlevel0.target -> poweroff.target
	/usr/lib/systemd/system/runlevel1.target -> rescue.target
	/usr/lib/systemd/system/runlevel2.target -> multi-user.target
	/usr/lib/systemd/system/runlevel3.target -> multi-user.target
	/usr/lib/systemd/system/runlevel4.target -> multi-user.target
	/usr/lib/systemd/system/runlevel5.target -> graphical.target
	/usr/lib/systemd/system/runlevel6.target -> reboot.target
	/usr/lib/systemd/system/emergency.target
	...

	$ man systemd.special
	graphical.target: graphical multi-user system
	multi-user.target: non-graphical multi-user system
	rescue.target: basic system single-user mode
	emergency.target: most minimal system
	...

## Process-management
### Process listing
The boot process

	$ ps 1
	/usr/lib/systemd/systemd --switched-root --system --deserialize 18

Children of the first process

	$ ps -f --ppid 1
	/usr/lib/systemd/systemd-journald
	/usr/lib/systemd/systemd-udevd
	/usr/bin/rpcbind -w -f
	#...

All processes (-e) with full format (-f) display as ASCII art tree (--forest):

	$ ps -ef --forest

### Process control
* `command &` Start background job.
* `fg %<jobnumber>` / `%<jobnumber>` Run job in the foreground.
* `Ctrl-z` Suspend job.
* `%<jobnumber>` Resume job.
* `%<jobnumber>&` Resume job in background.
* `jobs -l` Display job number, pid, process status, process name.

### Process signals
* `kill -l` Display signal numbers.
* `kill -s SIGSTOP pid` Suspend process.
* `kill -s SIGCONT pid` Resume process.

## User-management

### Basic commands

* `su <user>` Switch user.
* `su - <user>` Switch user and load the user environment.
* `su` Switch to the root user.
* `useradd` Add a new user. The user is inserted to /etc/passwd, /etc/shadow.
* `passwd` Set a user password.
* `usermod` Modify user account.
* `userdel` Delete user. By default the home directory is not deleted.
* `groupadd` Add a new group. The group is inserted to /etc/group, /etc/gshadow.
* `groupdel` Delete group.
* `gpasswd` Set a group password.
* `newgrp` Log in to a new group.
* `groups` / `id -Gn` Print groups of the current user. 
* `chgrp <group> <file>` Change the group owner of a file.

Example

	$ groupadd grp_fbi
	$ groupadd grp_sheriff
	$ groupadd grp_twnpks

	$ useradd --comment "Dale Cooper" -g grp_fbi -G grp_twnpks --create-home dcooper
	$ useradd --comment "Harry Truman" -g grp_sheriff -G grp_twnpks --create-home htruman

	$ passwd dcooper
	$ passwd htruman

### /etc/sudoers

Permit a user to run 'sudo \<command\>'.

	$ man sudoers

Edit the file with `sudo visudo`.

	[dummy_user]$ chown other_user foo.bar
	dummy_user is not in the sudoers file.  This incident will be reported.

	[admin]$ sudo visudo
	#insert and save:
	dummy_user ALL=/bin/chown

Now 'dummy\_user' is allowed to run sudo chmod.


### /etc/passwd

Contains all user and system accounts.

	$ man 5 passwd

Examples

* root user: `root:x:0:0:root:/root:/bin/bash`
	* name: root
	* password: x
	* UID: 0
	* GID: 0
	* comment: root
	* home: /root
	* shell: /bin/bash

	'password: x' means that the encrypted password is stored in /etc/shadow.

* sync user: `sync:x:5:0:sync:/sbin:/bin/sync`
	* UID: 5
	* GID: 0
	* shell: /bin/sync

	The sync user can only run /bin/sync.

* daemon user: `daemon:x:2:2:daemon:/sbin:/sbin/nologin`
	* UID: 2
	* GID: 2
	* shell: /sbin/nologin

	The daemon user can not login '/sbin/nologin'.

		$ nologin
		This account is currently not available.

* Anonymous user: `nobody:x:65534:65534:Kernel Overflow User:/:/sbin/nologin`
	* name: nobody
	* UID: 65534
	* GID: 65534
	* comment: Kernel Overflow User

### /etc/shadow

Shadowed password file. Contains the encrypted password for all users.

	$ man 5 shadow

Example

* `jdoe:$x$xxxxxx...x/::0:99999:7:::`
	* name: jdoe
	* enrcypted password: "$x$x...x/"
	* last changed: - (password aging feature disabled)
	* min age: 0 (no minimum password age)
	* max age: 99999 (the login password has never to be changed)
	* warn period: 7 days
	* inactive period: - (no enforcement of inactivity period)
	* account expire: - (account will never expire)
	* reserved: -

More about he password format
	
	$ man 3 crypt
	$ man 5 crypt

### /etc/group

Contains all user groups.

	$ man group

Example

* `wheel:x:10:jdoe`
	* name: wheel
	* password: x
	* GID: 10
	* user list: jdoe

	'password: x' means that the encrypted password is stored in /etc/gshadow.

### /etc/gshadow

Shadowed information group accounts.

	$ man gshadow

entry format: group\_name:password:admins:members

Example

* `wheel:::jdoe`
	* name: wheel
	* password: - (only group members can gain group permissions)
	* admins: - (no admins that can change the password or the members of the group)
	* members: jdoe (members that can access the group without a password)

### /etc/skel/

Files put here are copied to the home directory of a new user.

#### /etc/shells

Path name of all login shells available on the system.

### /etc/login.defs

Definitions for user account creation.

	#
	# Min/max values for automatic uid selection in useradd
	#
	UID_MIN                  1000
	UID_MAX                 60000
	# System accounts
	SYS_UID_MIN               201
	SYS_UID_MAX               999

	#
	# Min/max values for automatic gid selection in groupadd
	#
	GID_MIN                  1000
	GID_MAX                 60000
	# System accounts
	SYS_GID_MIN               201
	SYS_GID_MAX               999

## Disk-partitioning

`fdisk /dev/<device-name>` Manage partitions on device.

fdisk
* m - menu help
* n - add partition
* t - change partition type
* w - write partition table

### Using a file as a block device

1. `dd if=/dev/zero of=./device.img bs=1G count=1` Creates a 1 GByte file. 
1. `losetup -fP device.img` Associate the file with a free loop device.
1. `losetup -l` Find the loop device for that file.
1. `fdisk /dev/loop<number>` Selecting the right loop device to start partitioning.

Example how it can look for 4 partitions:

	/dev/loop0
	/dev/loop0p1
	/dev/loop0p2
	/dev/loop0p3
	/dev/loop0p4

Create a file systems on some partitions:

	$ mkfs.xfs /dev/loop0p1
	$ mkfs.ext4 /dev/loop0p3
	$ mkfs.fat /dev/loop0p4

Finally mount a file system:

	$ mount /dev/loop0p3 <directory>

Info about block devices, the filesystem and mountpoint

* `lsblk -f`
* `findmnt`

## Network-analysis

### Network layer models

| Layer | OSI          | TCP/IP      | Protocols              |
| ----: | ------------ | ----------- | ---------------------- |
| 7     | Application  | Application | DNS, FTP, SMTP, TELNET |
| 6     | Presentation |             |                        |
| 5     | Session      |             |                        |
| 4     | Transport    | Transport   | TCP, UDP               |
| 3     | Network      | Internet    | IPv4, IPv6             |
| 2     | Security     |             |                        |
| 1     | Physical     |             |                        |


### Basic network analysis

* `ping` Test if a network host responses to a ECHO request

		$ ping -c 3 google.com
		PING google.com (172.217.22.78) 56(84) bytes of data.
		64 bytes from fra15s17-in-f14.1e100.net (172.217.22.78): icmp_seq=1 ttl=128 time=21.7 ms
		64 bytes from fra15s17-in-f14.1e100.net (172.217.22.78): icmp_seq=2 ttl=128 time=24.2 ms
		64 bytes from fra15s17-in-f14.1e100.net (172.217.22.78): icmp_seq=3 ttl=128 time=22.9 ms

	Domain names like `fra15s17-in-f14.1e100.net` that end with `1e100.net` are servers owned by Google. '1e100' is the scientific notation of 1 googol.

* `host` DNS lookup.

	Reverse lookup:

		$ host 172.217.23.142
		142.23.217.172.in-addr.arpa domain name pointer fra16s18-in-f14.1e100.net.
		142.23.217.172.in-addr.arpa domain name pointer fra16s18-in-f142.1e100.net.

	For the IP address `172.217.23.142` the PTR record `142.23.217.172.in-addr.arpa` is looked up. It points to two names.
	
	The google website can no be visited by typing these names in the browser address bar. The google server doesn't accept these names.

	The name may reveal the Inernet Service Provider of an IP Address.

* `whois` Query a database that stores information about internet resources such as an IP address

		$ whois -r --sources ARIN-GRS 172.217.21.238
		inetnum:        172.217.0.0 - 172.217.255.255
		org:            GOGL
		netname:        GOOGLE

* `nslookup` Lookup information over DNS.

	Example interactive mode

		$ nslookup
		> set debug
		> server ns11.constellix.com
		> kernel.org
		Name:	kernel.org
		Address: 198.145.29.83

* `ip` Show / manipulate routing, network devices, interfaces and tunnels.

		$ ip a
		1: lo: <LOOPBACK,UP,LOWER_UP>
		2: ens33: <BROADCAST,MULTICAST,UP,LOWER_UP>
		...

* `ifconfig` Configure a network interface.

		$ ifconfig
		ens33: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>
		lo: flags=73<UP,LOOPBACK,RUNNING>
		...

	Display the config setting for the ethernet interface
	
		$ cat /etc/sysconfig/network-scripts/ifcfg-ens33
		TYPE="Ethernet"
		BOOTPROTO="dhcp"
		NAME="ens33"
		DEVICE="ens33"
		ONBOOT="yes"
		...
* `hostname` show or temporarily change the hostname

	Edit /etc/hostname to permanently set the hostname.

* `/etc/hosts file` - Static table lookup for hostnames.
* `traceroute` Print the route packets trace to network host.
* `nmap` Network exploration tool and security / port scanner.
* `tcpdump` Dump traffic on a network.

	Example capture HTTP traffic, stop after 1000 packets
	
		$ sudo tcpdump -i any -c 1000 -w traffic.pcap port http
	
	`tcpdump -r traffic.pcap` reads the file. [wireshark](<https://www.wireshark.org/>) is a tool to analyze the network traffic also from a pcap file.

### /etc/services

Network services. Example entries:

	# service-name  port/protocol  [aliases ...]   [# comment]
	ftp             21/tcp
	ftp             21/udp          fsp fspd
	ssh             22/tcp                          # The Secure Shell (SSH) Protocol
	ssh             22/udp                          # The Secure Shell (SSH) Protocol
	telnet          23/tcp
	telnet          23/udp
	http            80/tcp          www www-http    # WorldWideWeb HTTP
	http            80/udp          www www-http    # HyperText Transfer Protocol
	http            80/sctp                         # HyperText Transfer Protocol
	https           443/tcp                         # http protocol over TLS/SSL
	https           443/udp                         # http protocol over TLS/SSL
	https           443/sctp                        # http protocol over TLS/SSL
	quake           26000/tcp
	quake           26000/udp
	mdqs            666/tcp doom                    # doom Id Software
	mdqs            666/udp doom                    # doom Id Software

## IP-Internet-Protocol

* Routing datagrams from a source host interface to a destination host interface across one or more IP networks.
* Provide an addressing system.
* RFC 791 - Internet Protocol.
* Intelligence in the network is located in the end nodes.
* Best-effort delivery.
* Unreliable.
* Out-of-order delivery.
* End-to-end principle.
* Connectionless.
* Header checkum test.
* 32-bit address space provides 2^32 unique addresses.
* Special address blocks for private networks (c. 18 million) and multicast addresses (c. 270 million).

### IP protocol history

* Original IPv4 design: 1. octet network id, rest host ids.
* 256 network ids quickly found to be inadequate.
* Revised system 1981: Classful networking. Classes A, B and C with different length of the network id part in the address.
* 1985 deviding existing classful networks into subnets.
* 1987 introducing variable-length subnet masks.
* 1993 Classless Inter-Domain Routing (CIDR).
* Permit repartitioning of any address space so that smaller or larger address blocks can be assigned to users.

### IPv4 address fromat

Notation

* 127.0.0.0 - 127.255.255.255
	* Class A 127.0.0.0
	* 127.0.0.0/8
	* local host loopback address range

* 100.64.0.0 - 100.127.255.255
	* 100.64.0.0/10

* 10.0.0.0 - 10.255.255.255, 172.16.0.0 - 172.31.255.255, 192.168.0.0 - 192.168.255.255
	* These address ranges are used for local communication within a private network.
	* They are not routable in the public Internet. Private hosts cannot directly communicate with public networks.
	* Without NAT at the routing gateway a host 192.168.X.X would not be able to communicate with a public server.

Subnets

The first address in a subnet identifies the subnet itself. The last address is the broadcast address for all devices on the subnet.

| subnet          | network id   | broadcast address | example host address  |
| --------------  | ------------ | ----------------- | --------------------- |
| 192.168.5.0/24  | 192.168.5.0  | 192.168.5.255     | 192.168.5.42          |
| 192.168.0.0/16  | 192.168.0.0  | 192.168.255.255   | 192.168.1.255         |
| 203.0.113.16/28 | 203.0.113.16 | 203.0.113.31      | 203.0.113.18          |


### IPv4 packet header

* 16-bit IPv4 header checksum used for error-checking of the header.
* Routers decrease the TTL field. Consequently the router must calculate a new checksum.
* The source address and the destination address may be changed in transit by a NAT device.

### IP Address Allocations


Users get their IP address from a Internet Service Provider (ISP). And the ISPs get IP addresses from a Regional Internet Registry (RIR). There are five RIRs covering the globe:
* AFRINIC - African Network Information Centre
* APNIC - Asia Pacific Network Information Centre
* ARIN - American Registry for Internet Numbers
* LACNIC - Latin America and Caribbean Network Information Centre
* RIPENCC - Réseaux IP Européens Network Coordination Centre+

On top of the hierarchy is the Internet Assigned Numbers Authority (IANA). They make IP address allocations to the RIR.

### IPv4 address exhaustion

* IPv4 has insufficient number of publicly routable addresses.
* Transition from classful network addressing to CIDR delayed exhaustion of addresses substantially.
* NAT: ISPs can masquerade private network address space with one publicly routable IPv4 address on the Internet interface of a main Internet router.
* IPv6 protocol is the successor of IPv4.

### NAT

The router NAT replaces the private source address with the public source address in the IP header for sending the datagram over the internet to the destination address.
When receiving the answer datagram at the public interface of the router the public destination address is mapped to the private destination address.
With NAT even a large network can be created and connected to the Internet with a single public IP address.

## Byte-Multiples

	SI units
	1 Kilobyte (KB)                = 1000       bytes
	1 Megabyte (MB)  = 1000 * 1000 = 1000000    bytes
	1 Gigabyte (GB)  = 1000^3      = 1000000000 bytes
	
	IEC units
	1 Kibibyte (KiB)               = 1024       bytes
	1 Mebibyte (MiB) = 1024 * 1024 = 1048576    bytes
	1 Gibibyte (GiB) = 1024^3      = 1073741824 bytes

Calculation examples

100 MB  = 100 * 1000 KB  = 100 * 1000 * 1000 bytes = 100000000 bytes\
100 MiB = 100 * 1024 KiB = 100 * 1024 * 1024 bytes = 104857600 bytes\
100 MiB = 104857600 / 1000^2 bytes = 104,8576 MB

The ls command uses powers of 1024 by default. It also has an option to print the SI unit.

	$ ls -l disk1.img
	104857600
	$ ls -l -h disk1.img
	100M
	ls -l --si disk1.img
	105M

## Numpad
### Default Linux behavior
Num Lock off: after system boot-up the Num Lock state is off and the numpad keys move the cursor position. When holding the shift key, the numbers are entered instead.

Num Lock on: numbers are entered. Holding shift and using the numpad keys moves the cursor and allows to highlight objects. For instance, pressing '7' (Pos1) on the numpad highlights the current line from the beginning to the cursor position.

However, by pressing '7' (Pos1) again, the highlighted area is replaced by the number 7. MS Windows users would expect that the highlighted area is not replaced.

On Laptops where a full PC keyboard is missing the default behavior of Numpad can be annoying when used to the MS Windows behavior.

### Switch to MS Windows behavior

Temporary solution

	#german keyboard layout
	$ setxkbmap de -option numpad:microsoft

Persistent solution 1

	$ gnome-tweaks

Mark the checkbox 'Keyboard & Mouse - Additional Layout Options - Miscellaneous compatibility options - Num Lock on: ...'.

Persistent solution 2

	$ gsettings get org.gnome.desktop.input-sources xkb-options
	['terminate:ctrl_alt_bksp']
	$ gsettings set org.gnome.desktop.input-sources xkb-options['terminate:ctrl_alt_bksp', 'numpad:microsoft']"

Or use a GUI tool

	$ dconf-editor


## Tools
### Console Diff tools

* diff
* sdiff
* vimdiff

### Programming editors

There is [Visual Studio Code](<https://code.visualstudio.com/> "Visual Studio Code editor"). However this editor collects telemetry data and sends it to Microsoft by default. It uses a proprietary license.

Then there is [VSCodium](<https://vscodium.com//> "VSCodium editor"). This uses the same code base as the Visual Studio Code editor but it has telemetry off by default and it uses the MIT license for the binaries.

### Dictionary and translation

Offline dictionary or language translator solution **GoldenDict**. Install it via the package manager to get the newest version. On CentOS:

	# CentOS
	$ yum install goldendict

The Homepage seems to contain out-of-date links leading to suspicious websites.

Free single language english dictionary: get 'WordNet3.0\_1.0.zip' from <https://sourceforge.net/projects/goldendict/files/dictionaries/>.

On <https://www.babylon-software.com/free-dictionaries/> there are many free dictionaries for download. For instance, there is a English-German and German-English translation dictionary.

The dictionaries can be imported into GoldenDict.

## Miscellaneous
### General work flow
* make a copy any file, variable, etc. before editing it
* try commands in a VM first
* check if the command offers a dry-run option to simulate the operation
* read the man pages

## References

* [Linux Foundation Referenced Specifications](<https://refspecs.linuxbase.org/>)
* [ORACLE Operating Systems Documentation](<https://docs.oracle.com/en/operating-systems/index.html>)
* [The Linux Information Project](<http://www.linfo.org/index.html>)
* [ArchWiki](<https://wiki.archlinux.org/>)
* [Command line reference - Database and OS scripting](<https://ss64.com/>)
* [The Grymoire - home for UNIX wizards](<https://www.grymoire.com/Unix/index.html1>)
* [THE UNIX CD BOOKSHELF](<https://docstore.mik.ua/orelly/unix/index.htm>)
* [Red Hat Enable Sysadmin](<https://www.redhat.com/sysadmin/>)
* [nixCraft](<https://www.cyberciti.biz/tips/>)
* [IBM Linux Tutorials](<https://developer.ibm.com/technologies/linux/tutorials/>)
* [THE GEEK DIARY](<https://www.thegeekdiary.com/>)
* [LVM HOWTO](<https://tldp.org/HOWTO/LVM-HOWTO/>)
* [Product Documentation for Red Hat Enterprise Linux 8](<https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/8/>)
* [CentOS Wiki](<https://wiki.centos.org/>)
* [ Internet Assigned Numbers Authority (IANA)](<https://www.iana.org/>)
* [Wikipedia](<https://www.wikipedia.org/>)
