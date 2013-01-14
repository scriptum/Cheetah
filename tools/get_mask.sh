#!/bin/sh
QUALITY=""
while [ $# -gt 0 ]
do
	if [ "$1" = "-q" ]
	then
		shift
		QUALITY="-quality $1"
	else
		echo Processing "$1"
		img="${1%.png}.jpg"
		img_mask="${1%.png}_mask.jpg"
		img_mask2="${1%.png}_mask.png"
		convert "$1" $QUALITY "$img"
		convert "$1" $QUALITY -channel matte -separate "$img_mask"
		convert "$1" -channel matte -separate "$img_mask2"
		img_size=`stat -c%s "$img"`
		size_jpg=`stat -c%s "$img_mask"`
		size_png=`stat -c%s "$img_mask2"`
		if [ $size_jpg -gt $size_png ]
		then
			rm "$img_mask"
			sum_size=`expr $size_png + $img_size`
		else
			rm "$img_mask2"
			sum_size=`expr $size_jpg + $img_size`
		fi
		if [ $sum_size -gt `stat -c%s "$1"` ]
		then
			rm "$img" "$img_mask2" "$img_mask" 2> /dev/null
		fi
	fi
	shift
done