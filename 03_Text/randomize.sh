#!/bin/dash

exit_handler() {
    trap - EXIT
    test -r "$tmpfile1" && rm -vf "$tmpfile1"
    test -r "$tmpfile2" && rm -vf "$tmpfile2"
    test -r "$tmpfile3" && rm -vf "$tmpfile3"
    test -r "$tmpcolor" && rm -vf "$tmpcolor"
}

trap exit_handler EXIT HUP INT QUIT PIPE TERM

if [ "$#" -gt "0" ]; then
    ospeed=$1
else
    ospeed=0.01
fi

tput init
tput clear

tmpfile1=`mktemp`

cat | hexdump -v -e '/1 "%02X\n"' >$tmpfile1

column_no=0
line_no=0

tmpfile2=`mktemp`
while read line; do 
    if [ "$line" = "0A" ]; then
        column_no=0
        line_no=$(( $line_no + 1 ))
    else 
        column_no=$(( $column_no + 1 ))
        echo "$line" "$line_no" "$column_no" >>$tmpfile2
    fi
done <$tmpfile1
rm $tmpfile1

tmpfile3=`mktemp`
shuf <$tmpfile2 >$tmpfile3
rm $tmpfile2

colors=`tput colors`
tmpcolor=`mktemp`
while read char line col; do
    tput cup $line $col
    dd if=/dev/random of=$tmpcolor bs=1 count=1 > /dev/null 2>&1
    byte=$( od -tu1 -An < $tmpcolor )
    tput setaf $(( $byte % $colors ))
    /bin/echo -n -e "\x$char"
    sleep $ospeed
done <$tmpfile3

tput cup $line_no 0
rm $tmpfile3
rm $tmpcolor
