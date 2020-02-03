# cpio - copy files to and from archives

cpio archives can store linux file attributes. This is interesting for creating an exact copy of the files on another computer or disk drive for example.

The Linux kernel uses the cpio-format for the initramfs archive.

cpio provides different operation modes.

* -o, --create

    Copy-out.

    Read list of file names from stdin and create on stdout an archive containing those files.
* -i, --extract

    Copy-in.

    Read the archive from stdin and extract files from it on stdout.
* -p, --pass-through

    Pass-through.

    Read list of file names from stdin and copy them to the specified dir.

CPIO supports different formats, for example the "crc" format described in the man page as:

    crc   The new (SVR4) portable format with checksum added.

## Copy-out: creating an archive

Create a cpio archive of a directory with regular files, sub-directories and symlink.

    $ tree workdir/
    workdir/
    ├── empty/
    ├── foo
    └── sub
        ├── foo -> ../foo
        └── sub/
            └── bar

Using the 'find' command to create the list with the file names:

`$ find workdir | cpio --create --format=crc > archive.cpio`

The same result can be achieved by using different syntax depending what is prefered. The following commands are equivalent:


`$ find workdir | cpio -o --format=crc > archive.cpio`

`$ find workdir | cpio -o --format=crc -F archive.cpio`

`$ find workdir | cpio -o -H crc -O archive.cpio`

List the content of the archive:

`$ cat archive.cpio | cpio -t -v`

    drwxrwxr-x   4 johndoe johndoe        0 Dec 28 13:05 workdir
    drwxrwxr-x   3 johndoe johndoe        0 Dec 28 12:31 workdir/sub
    drwxrwxr-x   2 johndoe johndoe        0 Dec 28 12:32 workdir/sub/sub
    -rw-rw-r--   1 johndoe johndoe        4 Dec 28 12:32 workdir/sub/sub/bar
    lrwxrwxrwx   1 johndoe johndoe        6 Dec 28 12:31 workdir/sub/foo -> ../foo
    drwxrwxr-x   2 johndoe johndoe        0 Dec 28 12:31 workdir/empty
    -rw-rw-r--   1 johndoe johndoe        4 Dec 28 12:30 workdir/foo
    1 Block

The archive listing produced by cpio provides extensive information like calling `ls -l` in the _workdir/_ folder.

### Integrity check
If the archive was moved over the network it might be a good idea to ensure that the data was not damaged:

`$ cat archive.cpio | cpio --extract --only-verify-crc`

cpio prints an error message for each corrupt file.

_**It seems that the file headers in the archive are not protected since there is no error message after manipulating the file name.**_

### Pitfall

Be cautious about the file names passed to cpio. For example creating the following archive will succeed. However, trying to extract it will fail.

`$ echo workdir/foo | cpio -o -F archive.cpio`

`$ cpio -t --quiet -F archive.cpio `

    workdir/foo

Looks fine, but it's not!

`$ cpio --extract -F archive.cpio -D dest`

    cpio: workdir/foo: Funktion »open« fehlgeschlagen: Datei oder Verzeichnis nicht gefunden

cpio can not extract the file _workdir/foo_ because there is no directory _workdir_ in the _dest_ folder. It did not happen before because 'find' prints also the directory names. So one could add the directory name to the custom file list or use the '-d' parameter to let cpio create a directory when needed. This one works:

`$ cpio --extract -d -F archive.cpio -D dest`

## Copy-in: extracting an archive

The following command extracts the cpio archive. It restores the _workdir_ folder in the specifed destination.

`$ cat archive.cpio | cpio --directory=dest --extract`

The same command using shorter syntax:

`$ cpio -D dest -i -F archive.cpio`

**Always be aware not to overwrite or accidentally delete the original data !!!**

## Pass-through: copy files

For example I want to copy the workdir folder to another machine. Therefore I could copy the folder to an archive first, then extracting the archive to my destination folder or mountpoint. An alternative approach would be to use the pass-through method to directly copy the files to the destination.

`$ find workdir | cpio --pass-through --preserve-modification-time dest/`

## Compression

It could make sense to compress the cpio archive. Therefore the cpio stream can be piped to a compression utility, like 'xz' in the following example.

### Create a compressed cpio archive
`find workdir | cpio -o | xz > archive.cpio.xz`

### Extract a compressed cpio archive
Restore the original workdir files:

`xz --decompress archive.cpio.xz --stdout | cpio --directory=dest --extract`

Or with short options:

`xz -d archive.cpio.xz -c | cpio -D dest -i`

## Splitting large data

If there are many files that should be copied to a cpio archive it can be helpful to split the archive in smaller parts for different reasons like when the file system has file size restrictions or because it is more handy to work with small files. This is possible with the 'split' program from GNU coreutils.

This examples show how a 5,0M 'blob' from _workdir_ is passed to cpio and splitted up:

`$ find workdir | cpio --create --format=crc | split -b 2M - workdir.cpio_`

Result:

    $ ls -1sh workdir.cpio_a?
    2,0M workdir.cpio_aa
    2,0M workdir.cpio_ab
    1,1M workdir.cpio_ac

Restore the original data from the splitted cpio archive:

`$ cat workdir.cpio_a? | cpio --extract -D dest`

## References

[GNU cpio manual](https://www.gnu.org/software/cpio/manual/)

[cpio article on linux journal](https://www.linuxjournal.com/article/1213)

[The cpio format used for the initramfs](https://www.kernel.org/doc/html/latest/driver-api/early-userspace/buffer-format.html)

## Remarks
* Read the man page `man cpio`
* After creating the archive, test if it's valid and complete.
* A small mistake like a typo - and this will happen - can lead to total, unrecoverable data loss.
