[Model Train Speedometer](http://baijs.com/model-train-speedometer)

==================================================

What you need to build your own version of model-train-speedometer
--------------------------------------

In order to build model-train-speedometer, you need to have Node.js/npm, and git 1.7 or later installed.


How to build your own model-train-speedometer
----------------------------

First, clone a copy of the main imageCreator git repo by running:

```bash
git clone git://github.com/wieringen/model-train-speedometer.git
```

Install the grunt-cli package so that you will have the correct version of grunt available from any project that needs it. This should be done as a global install:

```bash
npm install -g grunt-cli
```

Enter the model-train-speedometer directory and install the Node dependencies, this time *without* specifying a global install:

```bash
cd model-train-speedometer && npm install
```

Make sure you have `grunt` installed by testing:

```bash
grunt -version
```

Then, to get a complete, minified (w/ Uglify.js), linted (w/ JSHint) version of model-train-speedometer, type the following:

```bash
grunt
```

The built version of model-train-speedometer will be put in the `dist/` subdirectory, along with the minified copy and associated map file.


Questions?
----------

If you have any questions, please feel free to email [me](mailto:wieringen@gmail.com).


