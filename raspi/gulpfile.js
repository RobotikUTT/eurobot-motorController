var gulp = require('gulp');
var sourcemaps = require('gulp-sourcemaps');
var babel = require('gulp-babel');
var watch = require('gulp-watch');
var plumber = require('gulp-plumber');


gulp.task('build', function () {
    gulp.src(['src/**/*.js', '!src/controlPannel/public/*.js'])
        .pipe(plumber({
            handleError: function (err) {
                console.log(err);
                this.emit('end');
            }
        }))
        .pipe(sourcemaps.init())
        .pipe(babel())
        .pipe(sourcemaps.write('.'))
        .pipe(gulp.dest('build/src'));

    gulp.src('src/controlPannel/public/*')
        .pipe(gulp.dest('build/src/controlPannel/public'));
});

gulp.task('default', ['build'], function() {
    gulp.watch('src/**/*.js', ['build']);
});


