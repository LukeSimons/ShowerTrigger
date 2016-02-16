echo "Now initialising a Git Commit"

git init
git status

while true; do
    echo "Please enter FileNames to add or enter 'done' or 'exit': "
    read input_variable

    if [ "$input_variable" == "done" ]; then
      break
    elif [ "$input_variable" == "exit" ]; then
      exit
    else    
        git add $input_variable
        echo "$input_variable has been added" 
    fi
done


while true; do
    echo "Please enter commit message, when you're happy, enter 'done' or else 'exit': "
    read commit_message

    if [ "$commit_message" == "done" ]; then
      break
    elif [ "$commit_message" == "exit" ]; then
      exit
    else
        echo "Commit message is:" 
        echo "$commit_message"
    fi
done

git commit -m $input_variable
git remote add origin https://github.com/LukeSimons/ShowerTrigger.git
git push -u origin master
