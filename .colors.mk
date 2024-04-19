ifeq ($(shell uname), Linux)
# Reset colors
RESET:=`tput sgr0`

# Regular Colors
RED:=`tput bold setaf 1`
GREEN:=`tput bold setaf 2`
YELLOW:=`tput bold setaf 3`
BLUE:=`tput bold setaf 4`
MAGENTA:=`tput bold setaf 5`
CYAN:=`tput bold setaf 6`
WHITE:=`tput bold setaf 7`

else ifeq ($(shell uname), Darwin)
# Reset colors
RESET:=\033[0m

# Regular Colors
RED:=\033[1;31m
GREEN:=\033[1;32m
YELLOW:=\033[1;33m
BLUE:=\033[1;34m
MAGENTA:=\033[1;35m
CYAN:=\033[1;36m
WHITE:=\033[1;37m

endif