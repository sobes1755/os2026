#!/usr/bin/env bash

echo
echo "This script 1) creates a file (filename) filled with (filesize * MiB) zeroes,"
echo "2) creates a filesystem (ext2, ext3 or ext4) using the aforementioned file as a container,"
echo "3) creates a directory (dirname) to mount the filesystem to,"
echo "4) sudo mounts the filesystem to the directory,"
echo "5) sudo creates a directory \"test\" in the filesystem,"
echo "6) sudo changes the owner of the directory \"test\" to current user,"
echo "7) creates a 1KiB file \"qwerty.txt\" with random symbols \"QWERTY\" in the directory \"test\","
echo "8) creates a (hard) link to the file \"qwerty.txt\","
echo "9) creates a (osft) symbolic link to the file \"qwerty.txt\""
echo "10) and finally lists the contents of the directory \"test\" (with inodes' numbers)."

echo
read -p "Enter filename: " filename
read -p "Enter filesize (MiB): " filesize

if [ -z "$filename" ] || [ -z "$filesize" ]; then
	echo -e "\n\e[31mFilename and filesize cannot be empty!\e[0m"
	exit 1
fi

echo
echo "Creating file with data duplicator (dd)..."
echo

set -x    

if ! dd if=/dev/zero of="$filename" bs=1MiB count="$filesize"; then 
	{ set +x; } 2>/dev/null
	echo -e "\n\e[31mData duplicator (dd) failed!\e[0m"
	exit 1
fi

{ set +x; } 2>/dev/null    

PS3="Please enter your choice (1-3): "	

echo
echo "Select filesystem type:"
echo

select fstype in "ext2" "ext3" "ext4"
do	
	if [ -n "$fstype" ]; then
		break
	fi
	echo "Invalid option. Please try again."
done

echo
echo "Creating filesystem with mkfs..."
echo

set -x

if ! mkfs -t "$fstype" "$filename"; then
	{ set +x; } 2>/dev/null
	echo -e "\n\e[31mFilesystem maker (mkfs) failed!\e[0m"
	exit 1
fi

{ set +x; } 2>/dev/null

echo
read -p "Enter dirname to mount filesystem to: " dirname

if [ -z "$dirname" ]; then
	echo -e "\n\e[31mDirname cannot be empty!\e[0m"
	exit 1
fi

echo
echo "Creating directory..."

set -x

if ! mkdir "$dirname"; then
	{ set +x; } 2>/dev/null
	echo -e "\n\e[31mDirectory maker (mkdir) failed!\e[0m"
	exit 1
fi

{ set +x; } 2>/dev/null

echo "Sudo mounting filesystem to directory..."

set -x

if ! sudo mount "$filename" "$dirname" -o loop,nodev; then
	{ set +x; } 2>/dev/null
	echo -e "\n\e[31mMount (sudo mount) failed!\e[0m"
	exit 1
fi

{ set +x; } 2>/dev/null

echo
read -n 1 -s -r -p "Press a key to sudo make directory \"${dirname}/test\"..."
echo

set -x

if ! sudo mkdir "${dirname}/test"; then
	{ set +x; } 2>/dev/null
	echo -e "\n\e[31mMaking directory \"${dirname}/test\" (sudo mkdir) failed!\e[0m"
	exit 1
fi

{ set +x; } 2>/dev/null

echo
read -n 1 -s -r -p "Press a key to sudo change the owner of directory \"${dirname}/test\" to current user..."
echo

set -x

if ! sudo chown "$USER" "${dirname}/test"; then
	{ set +x; } 2>/dev/null
	echo -e "\n\e[31mChanging owner of directory \"${dirname}/test\" (sudo chown) failed!\e[0m"
	exit 1
fi

{ set +x; } 2>/dev/null

echo
read -n 1 -s -r -p "Press a key to create file \"${dirname}/test/qwerty.txt\" with random \"QWERTY\" symbols..."
echo

set -x

if ! tr -dc 'QWERTY' < /dev/random | head -c 1KiB > "${dirname}/test/qwerty.txt"; then
	{ set +x; } 2>/dev/null
        echo -e "\n\e[31mCreating file \"qwerty.txt\" failed!\e[0m"
	exit 1
fi

{ set +x; } 2>/dev/null

set -x;
cat "${dirname}/test/qwerty.txt"
{ set +x; } 2>/dev/null

echo
echo
read -n 1 -s -r -p "Press a key to create a (hard) link to \"${dirname}/test/qwerty.txt\"..."
echo

set -x

if ! ln "${dirname}/test/qwerty.txt" "${dirname}/test/hard.txt"; then
	{ set +x; } 2>/dev/null
	echo -e "\n\e[31mCreating hardlink (ln) failed!\e[0m"
	exit 1
fi

{ set +x; } 2>/dev/null

echo
read -n 1 -s -r -p "Press a key to create a (soft) symlink to \"${dirname}/test/qwerty.txt\"..."
echo

set -x

if ! ln -sr "${dirname}/test/qwerty.txt" "${dirname}/test/soft.txt"; then
	{ set +x; } 2>/dev/null
	echo -e "\n\e[31mCreating symlink (ln -s) failed!\e[0m"
	exit 1
fi

{ set +x; } 2>/dev/null

echo
read -n 1 -s -r -p "Press a key to list the contents of \"${dirname}/test\"..."
echo

set -x
ls -ali "${dirname}/test/"
{ set +x; } 2>/dev/null
