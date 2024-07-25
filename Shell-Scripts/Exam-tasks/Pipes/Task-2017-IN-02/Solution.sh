

read -r choise </dev/tty

case $choice in 
	'First')
		make=$(find . -type f -size 0 -exec rm -f {} \;)
		if [ -n make ] ; then
			echo "Error"
		fi
	;;
	'Second')
		made=$(find . -type f -exec stat -c "%s %n" + 2>/dev/null | sort -nr -k1 | head -n 5 | cut -f2 | xargs rm -f)
		if [ -n ${made} ]; then
			echo "error"
		fi
	;;
esac
