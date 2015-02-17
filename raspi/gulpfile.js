var gulp = require('gulp');
var sourcemaps = require('gulp-sourcemaps');
var babel = require('gulp-babel');
var watch = require('gulp-watch');

gulp.task('build', function () {
    gulp.src('src/**/*.js')
        .pipe(sourcemaps.init())
        .pipe(babel())
        .pipe(sourcemaps.write('.'))
        .pipe(gulp.dest('build/src'));
});

gulp.task('default', ['build'], function() {
    gulp.watch('src/**/*.js', ['build']);
});


