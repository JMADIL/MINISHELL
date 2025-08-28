#!/bin/bash

# Minishell Test Script
echo "=== Testing Minishell ==="

# Test 1: Basic command execution
echo "Test 1: Basic echo command"
echo 'echo "Hello World"' | ./minishell &
sleep 2
kill $! 2>/dev/null

# Test 2: Built-in pwd
echo "Test 2: pwd command"
echo 'pwd' | ./minishell &
sleep 2
kill $! 2>/dev/null

# Test 3: Built-in env (partial)
echo "Test 3: env command"
echo 'env | head -3' | ./minishell &
sleep 2
kill $! 2>/dev/null

# Test 4: Exit command
echo "Test 4: exit command"
echo 'exit' | ./minishell &
sleep 2

# Test 5: Invalid command
echo "Test 5: Invalid command"
echo 'nonexistentcommand' | ./minishell &
sleep 2
kill $! 2>/dev/null

echo "=== Tests completed ==="
