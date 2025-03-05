#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}


@test "Grep from bad pipe" {
    run "./dsh" <<EOF
cd | grep c
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed
    expected_output="dsh3>dsh3>cmdloopreturned0"

    # Debugging echo commands
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Exit stops pipe" {
    run "./dsh" <<EOF
pwd | cd . | exit | ls | grep c
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed
    expected_output="dsh3>cmdloopreturned0"

    # Debugging echo commands
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "7 pipes returns output" {
    run "./dsh" <<EOF
pwd | pwd | pwd | pwd | pwd | pwd | pwd
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    current_directory=$(pwd)

    # Expected output with all whitespace removed
     expected_output="${current_directory}dsh3>dsh3>cmdloopreturned0"

    # Debugging echo commands
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "8 pipes return no output" {
    run "./dsh" <<EOF
pwd | pwd | pwd | pwd | pwd | pwd | pwd | pwd
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed
     expected_output="dsh3>cmdloopreturned0"

    # Debugging echo commands
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "External command cd" {
    run "./dsh" <<EOF
cd bats | pwd
EOF

    # Strip all whitespace from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    current_directory=$(pwd)

    # Expected output with all whitespace removed
     expected_output="${current_directory}/batsdsh3>dsh3>cmdloopreturned0"

    # Debugging echo commands
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}
