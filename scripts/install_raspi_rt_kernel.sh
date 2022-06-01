#!/usr/bin/env bash

# https://github.com/kdoren/linux/wiki/Installation-of-kernel-from-deb-package-%28Raspberry-Pi-OS%29
# https://github.com/kdoren/linux/releases/tag/rpi_5.15.32-rt39

wget https://github.com/kdoren/linux/releases/download/rpi_5.15.32-rt39/linux-image-5.15.32-rt39-v7l+_5.15.32-1_armhf.deb
dpkg -i linux-image-5.15.32-rt39-v7l+_5.15.32-1_armhf.deb

KERN=5.15.32-rt39-v7l+

mkdir -p /boot/$KERN/o/
cp -d /usr/lib/linux-image-$KERN/overlays/* /boot/$KERN/o/
cp -dr /usr/lib/linux-image-$KERN/* /boot/$KERN/
[[ -d /usr/lib/linux-image-$KERN/broadcom ]] && cp -d /usr/lib/linux-image-$KERN/broadcom/* /boot/$KERN/
touch /boot/$KERN/o/README
mv /boot/vmlinuz-$KERN /boot/$KERN/
mv /boot/initrd.img-$KERN /boot/$KERN/
mv /boot/System.map-$KERN /boot/$KERN/
cp /boot/config-$KERN /boot/$KERN/

cat >> /boot/config.txt << EOF

[all]
kernel=vmlinuz-$KERN
# initramfs initrd.img-$KERN
os_prefix=$KERN/
overlay_prefix=o/$(if [[ "$KERN" =~ 'v8' ]]; then echo -e "\narm_64bit=1"; fi)
[all]
EOF
