function ergodic(){
        for file in ` ls $1 `
        do
				echo "$file" | grep ".asp"
                if [ $? -eq 0 ]
                then
                        rm $file
						ln -s ../html/$file ./$file                
                fi
        done
}

cd $1
pwd
ergodic ./
cd ..
