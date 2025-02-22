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


@test "Check 7 arguments" {
    run "./dsh" <<EOF
echo "hi " im the 5th 6th 7th
EOF

    # Assertions
     # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="hi  im the 5th 6th 7thdsh2> dsh2> cmd loop returned 0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
}

@test "Check 8 arguments" {
    run "./dsh" <<EOF
echo "hi " im the 5th 6th 7th 8th
EOF

    # Assertions
     # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh2> Too many argumentsdsh2> cmd loop returned 0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
}


@test "cd->pwd" {
    run "./dsh" <<EOF
cd
pwd
EOF

    # Assertions
     # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="/home/joel/SysProg-Class/assignments/4-ShellP2/starterdsh2> dsh2> dsh2> cmd loop returned 0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
}

@test "cd foo->pwd" {
    run "./dsh" <<EOF
cd foo
pwd
EOF

    # Assertions
     # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="Invalid directory: No such file or directory/home/joel/SysProg-Class/assignments/4-ShellP2/starterdsh2> dsh2> dsh2> cmd loop returned 0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
}

@test "cd bats->pwd" {
    run "./dsh" <<EOF
cd bats 
pwd
EOF

    # Assertions
     # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="/home/joel/SysProg-Class/assignments/4-ShellP2/starter/batsdsh2> dsh2> dsh2> cmd loop returned 0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
}

@test "cd bats extrarg->pwd" {
    run "./dsh" <<EOF
cd bats arg
pwd
EOF

    # Assertions
     # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="Too many args: Success/home/joel/SysProg-Class/assignments/4-ShellP2/starterdsh2> dsh2> dsh2> cmd loop returned 0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
}

@test "exit" {
    run "./dsh" <<EOF
exit
EOF

    # Assertions
     # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh2> cmd loop returned 0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
}
