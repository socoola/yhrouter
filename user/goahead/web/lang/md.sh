function ergodic(){
        for file in ` ls $1 `
        do
				echo "$file" | grep ".xml"
                if [ $? -eq 0 ]
                then
                        rm $file
						ln -s ../$file ./$file                
                fi
        done
}

cd $1
pwd
ergodic ./
cd ..
