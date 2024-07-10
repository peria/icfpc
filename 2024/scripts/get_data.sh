#!/bin/zsh

# echo "get ${problem}" | ./icfp/main.py > index.md

# problems=("lambdaman" "spaceship" "3d" "efficiency")
# for problem in ${problems}
# do
#     echo "get ${problem}" | ./icfp/main.py > ${problem}/index.md
# done

# for i in {1..20}
# do
#     command="get lambdaman${i}"
#     echo ${command} | ./icfp/main.py > lambdaman/problems/lambdaman${i}.txt
#     echo "lambdaman ${i} done"
# done
# command="get lambdaman21"
# echo ${command} | ./icfp/main.py --no-evaluate > lambdaman/problems/lambdaman21.icfp

for i in {1..12}
do
    command="get 3d${i}"
    echo ${command} | ./icfp/main.py > 3d/problems/3d${i}.md
    echo "3d ${i} done"
done

# for i in {1..25}
# do
#     command="get spaceship${i}"
#     echo ${command} | ./icfp/main.py > spaceship/problems/spaceship${i}.txt
#     echo "spaceship ${i} done"
# done

# for i in {1..13}
# do
#     command="get efficiency${i}"
#     echo ${command} | ./icfp/main.py --no-evaluate > efficiency/problems/efficiency${i}.icfp
#     echo "efficiency ${i} done"
# done
