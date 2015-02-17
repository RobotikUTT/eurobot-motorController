eurobot-2015
============

Fix gulp-watch:
    echo fs.inotify.max_user_watches=524288 | sudo tee -a /etc/sysctl.conf && sudo sysctl -p

Launch gulp for auto build with babel:
    gulp&

Launch nodemon for auto relaunch the script:
    nodemon build/src/main.js