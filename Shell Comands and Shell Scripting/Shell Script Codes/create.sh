#!/bin/sh

touch $1
chmod u+x $1
echo "#!/bin/bash" > $1

gedit $1

