#!/usr/bin/env bash

# Compiling test_parser
c++ -Wall -Werror -Wextra -std=c++98 -ggdb3 -D__DEBUG__ -I../ -I../../../includes/ -I../../server/ -I../../http/ ../ConfigParser.cpp ./test_parser.cpp -o test_parser
echo "`tput setaf 4``tput bold`[ TEST ]`tput sgr0` Parser ready for testing."

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed. Exiting."
    exit 1
fi

# Check if the script is invoked with the argument "compile"
if [ "$1" = "compile" ]; then
    exit 0
fi

test_dir="./test_files"
if [ ! -d "$test_dir" ]; then
    echo "Test directory '$test_dir' not found. Exiting."
    exit 1
fi

sub_dirs=(
    "brace_check"
    "client_body_size_check"
    "empty_check"
    "missing_check"
    "duplicate_check"
)

# Prompt user to select a test directory
echo "Select a test directory to run: "
echo "0. all"
for i in "${!sub_dirs[@]}"; do
    echo "$((i+1)). ${sub_dirs[i]}"
done

read -p "Enter the number corresponding to the test directory: " choice

# Run all test directories if no input or 0 is entered
if [ -z "$choice" ] || [ "$choice" -eq 0 ]; then
    echo "Running all test directories..."
    selected_dirs=("")
else
    # Validate user input
    if ! [[ "$choice" =~ ^[1-${#sub_dirs[@]}]$ ]]; then
        echo "Invalid input. Please enter a number between 1 and ${#sub_dirs[@]} or 0 to run all."
        exit 1
    fi

    # Run the selected test directory
    selected_dir="${sub_dirs[choice-1]}"
    echo "Running tests in $selected_dir directory..."
    selected_dirs=("$selected_dir")
fi

# Function to recursively find test files and run them
find_tests() {
    local dir="$1"
    for file in "$dir"/*; do
        if [ -d "$file" ]; then
            find_tests "$file"
        elif [ -f "$file" ]; then
            echo "Running test: $file"
            ./test_parser "$file"
            echo "--------------------------------"
        fi
    done
}

# Run the function to find and run tests in the selected directories
for dir in "${selected_dirs[@]}"; do
    if [ ! -d "test_files/$dir" ]; then
        echo "Test directory '$dir' not found. Skipping."
    else
        find_tests "test_files$dir"
    fi
done

echo "Test execution completed."