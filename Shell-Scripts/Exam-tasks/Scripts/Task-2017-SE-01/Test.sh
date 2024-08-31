#!/bin/bash

# Setup the test environment
mkdir -p testdir/subdir
touch testdir/file1
touch testdir/file2
ln testdir/file1 testdir/hardlink1
ln -s testdir/file1 testdir/symlink1
ln -s non_existent_file testdir/symlink_broken
touch testdir/subdir/file3
ln testdir/subdir/file3 testdir/subdir/hardlink2
ln -s testdir/subdir/file3 testdir/subdir/symlink2

# Test Case 1
echo "Test Case 1: Files with hard links greater than or equal to 2"
./script.sh testdir 2

# Test Case 2
echo "Test Case 2: No files with hard links greater than or equal to 3"
./script.sh testdir 3

# Test Case 3
echo "Test Case 3: List broken symlinks"
./script.sh testdir

# Test Case 4
echo "Test Case 4: No broken symlinks"
rm testdir/symlink_broken
./script.sh testdir

# Test Case 5
echo "Test Case 5: Empty directory"
mkdir emptydir
./script.sh emptydir

# Test Case 6
echo "Test Case 6: Non-existing directory"
./script.sh non_existing_dir

