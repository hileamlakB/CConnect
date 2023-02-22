# create test
echo "create client_1 pass1 \n
create client_2 pass2 \n" | ./client_build > result
count=$(grep -c "Account created successfully!!" result)

if [ $count -eq 2 ]; then
    echo "Create Test passed"
else
    echo "Create Test failed"
fi

# list test
echo "list \n" | ./client_build > result
count=$(grep -c "client_1" result)
count=$((count+$(grep -c "client_2" result)))

if [ $count -eq 2 ]; then
    echo "List Test passed"
else
    echo "List Test failed"
fi

# login 
echo -e "login client_1 pass1 \n" | ./client_build > result
count=$(grep -c "Login successful!!" result)

if [ $count -eq 1 ]; then
    echo "Login Test passed"
else
    echo "Login Test failed"
fi


# send test
echo -e "login client_1 pass1\nsend client_2 100" | ./client_build > result
count=$(grep -c "Message sent successfully!!" result)


if [ $count -eq 1 ]; then
    echo "Send Test passed"
else
    echo "Send Test failed"
fi

# receive test
echo -e "login client_2 pass2\nreceive" | ./client_build > result
count=$(grep -c "100" result)

if [ $count -eq 1 ]; then
    echo "Receive Test passed"
else
    echo "Receive Test failed"
fi