#!/bin/sh

TMP=/tmp/resize-psnr-$RANDOM.tga
MIN_PSNR=30
SCALE=50
COMPRESSED=0
DRY_RUN=0
while [ $# -gt 0 ]
do
	img="$1"
	case "$img" in
	-psnr)
		shift
		MIN_PSNR=$1
		echo "Setting minimum PSNR to $1"
	;;
	-scale)
		shift
		SCALE=$1
		echo "Setting scale to $1"
	;;
	-dry)
		DRY_RUN=1
		echo "Dry-run mode: actually does not do anything, just showing output"
	;;
	*)
		SIZE=`identify -format "%[fx:w]x%[fx:h]" "$img"`
		convert "$img" -adaptive-resize "$SCALE"% -resize $SIZE $TMP
		PSNR=`compare -metric psnr "$img" $TMP /dev/null 2>&1`
		if [ $0 ]
		then
			printf "PSNR of $img: \t $PSNR "
			if [ "$(echo $PSNR '>' $MIN_PSNR | bc -l)" -eq 1 ]
			then
				if [ -f "$img.bak" ]
				then
					printf " - Backup already exists! Skipping"
				else
					printf " - compressing"
					if [ $DRY_RUN -eq 0 ]
					then
						cp "$img" "$img.bak"
						convert "$img.bak" -adaptive-resize "$SCALE"% "$img"
					fi
					COMPRESSED=`expr $COMPRESSED + 1`
				fi
			else
				printf " - skipping (too low PSNR)"
			fi
			printf "\n"
		else
			echo "Error!"
			echo "$PSNR"
		fi
	;;
	esac
	shift
done
rm -f $TMP
echo "Complete! Compressed $COMPRESSED images"