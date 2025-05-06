#!/bin/bash

# Check if two arguments are provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <new_gtkbase_name> <new_app_name>"
    exit 1
fi

NEW_GTKBASE_NAME="$1"
NEW_APP_NAME="$2"
NEW_APP_NAME_CAPS=${NEW_APP_NAME^^}

# Replace "GtkBase" with NEW_GTKBASE_NAME in file contents (not filenames)
find . -type f -not -path '*/\.git/*' -exec sed -i "s/GtkBase/$NEW_GTKBASE_NAME/g" {} \;

# Replace "this_app_name" with NEW_APP_NAME in file contents
find . -type f -not -path '*/\.git/*' -exec sed -i "s/this_app_name/$NEW_APP_NAME/g" {} \;
find . -type f -not -path '*/\.git/*' -exec sed -i "s/THIS_APP_NAME/$NEW_APP_NAME_CAPS/g" {} \;

# Replace "this_app_name" with NEW_APP_NAME in filenames
find . -type f -name "*this_app_name*" -not -path '*/\.git/*' | while read -r file; do
    new_file=$(echo "$file" | sed "s/this_app_name/$NEW_APP_NAME/g")
    mv "$file" "$new_file"
done

echo "Replacements complete:"
echo "  'GtkBase' -> '$NEW_GTKBASE_NAME' in file contents"
echo "  'this_app_name' -> '$NEW_APP_NAME' in file contents and filenames"
