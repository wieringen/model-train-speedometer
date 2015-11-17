module.exports = ( grunt ) ->

    grunt.initConfig

        pkg : grunt.file.readJSON "package.json"

        meta :

            banner : '/*! <%= pkg.title || pkg.name %> - v<%= pkg.version %> - ' +
            '<%= grunt.template.today("yyyy-mm-dd") %>\n ' +
            '<%= pkg.homepage ? "* " + pkg.homepage + "\\n *\\n " : "" %>' +
            '* Copyright (c) <%= grunt.template.today("yyyy") %> <%= pkg.author.name %> <<%= pkg.author.email %>>;\n' +
            ' * Licensed under the <%= _.pluck(pkg.licenses, "type").join(", ") %> license */\n\n'

        #  Remove old build.
        #
        clean :

            docs :

                src : [ "dist" ]

        #  Copy the images and the index to the dist location.
        #
        copy :

            docs :

                files : [
                    { expand: true, cwd: "docs", src: "**/*", dest: "dist/docs", dot: true }
                    { expand: true, cwd: "node_modules/baijs", src: "css/**/*", dest: "dist/docs" }
                    { expand: true, cwd: "node_modules/baijs", src: "js/**/*",  dest: "dist/docs" }
                    { expand: true, cwd: "schematics", src: ['**/*',  '!**/libs/**'], dest: "dist/schematics", dot: true }
                    { expand: true, cwd: "schemes", src: "**/*", dest: "dist/schemes", dot: true }
                ]
        #  Replace image file paths in css and correct css path in the index.
        #
        replace :

            docs :
                src : [
                    "dist/docs/index.html"
                ]
                overwrite     : true
                replacements  : [
                    {
                        from : /@@bnr@@/ig
                    ,   to   : "<%= pkg.version %>"
                    }
                ,   {
                        from : /..\/node_modules\/baijs\//ig
                    ,   to   : ""
                    }
                ]

            files :
                src : [
                    "dist/docs/index.html"
                ]
                overwrite     : true
                replacements  : [
                    {
                        from : /\.\.\/schemes/ig
                    ,   to   : "schemes"
                    }
                ,   {
                        from : /\.\.\/schematics/ig
                    ,   to   : "schematics"
                    }
                ]

        #  Make a zipfile.
        #
        compress :

            dist :

                options :

                    archive: "dist/<%= pkg.name %>-<%= pkg.version %>.zip"

                files : [
                    { expand: true, cwd: 'dist/', src: ["schematics/**/*"]}
                    { expand: true, cwd: 'dist/', src: ["docs/**/*"] }
                    { expand: true, cwd: 'dist/', src: ["schemes/**/*"] }
                ]

        "ftp-deploy":

            docs:

                auth:

                    host    : "ftp.baijs.nl"
                    port    : 21
                    authKey : "<%= pkg.name %>"

                src: "dist/docs"
                dest: "/"

            files:

                auth:

                    host    : "ftp.baijs.nl"
                    port    : 21
                    authKey : "<%= pkg.name %>"

                src  : "dist/"
                dest : "/"
                exclusions: ["docs"]


    #  Load all the task modules we need.
    #
    grunt.loadNpmTasks "grunt-contrib-copy"
    grunt.loadNpmTasks "grunt-contrib-clean"
    grunt.loadNpmTasks "grunt-contrib-uglify"
    grunt.loadNpmTasks "grunt-text-replace"
    grunt.loadNpmTasks "grunt-contrib-compress"
    grunt.loadNpmTasks "grunt-ftp-deploy"
    grunt.loadNpmTasks "grunt-contrib-jshint"

    #  Distribution build
    #
    grunt.registerTask(

        "default"
    ,   [
            "clean:docs"
            "copy:docs"
            "replace:docs"
            "compress:dist"
        ]
    )

    #  Upload dist to baijs.nl
    #
    grunt.registerTask(

        "ftp"
    ,   [
            "default"
            "replace:files"
            "ftp-deploy:docs"
            "ftp-deploy:files"
        ]
    )
