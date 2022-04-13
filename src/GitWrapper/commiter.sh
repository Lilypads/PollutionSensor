git init 
git branch -M main
git add *.pol
git commit -m "AutoMessage"
git remote remove origin
git remote add origin git@github.com:BodeanTheZealous/PollutionSensorData.git
GIT_SSH_COMMAND='ssh -i ~/.ssh/test_private_key'
git pull origin main
git push origin main
