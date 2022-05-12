

# Eye-Wink
# User Manual and Documentation - Pre-Alpha release

By Naser Al Madi (from 2016)

## Introduction

Eye-Wink is an open-source tool for opening/visualizing/analyzing eye-tracker recorded
data files. The project aims at making eye-tracking research tools accessible and easily
available, and therefore this project will always be free to use and distribute (giving credit to
the creators and contributors). As this is a community driven project, your collaboration,
feedback, and suggestions are most welcome!

## Pre-Alpha Release

Pre-Alpha means that this software has not been tested completely yet. Using it at this
stage could cause damage to your files, therefore it is advised to work on a copy of your
date while you keep the original files somewhere safe.

Please, let me know if you run into any problems or bugs during the use of this release, as
it would be helpful to discover and fix problems at this stage. An issue documenting the
details of the bug would be much appreciated.


## Installation

To install Eye-wink, start by downloading “Eye-Wink-Release-Pre-Alpha.zip” to your
computer. Currently, Windows 7/8/10 are supported with Mac and Linux expected soon. 
After that, unzip the downloaded file to any directory, and you should get
the files shown in Figure 1.

```
Figure 1: contents of unzipped “Eye-Wink-Release-Pre-Alpha” directory.
```
The directory contains the executable “eye-wink.exe” and sample data files in the
directories “ASCII” which contains the ASCII exported eye-tracker files, and “runtime” which
is copied from the eye-tracker data files as is.

## How can I open my own experiment data files?

Easy! First, you should convert your experiment data to ASCII format (using the eye-tracker
provided software). Second, you should copy the folder “runtime” from your experiment to
the working directory (directory where Eye-wink.exe is located at).


## Using Eye-Wink

For the purpose of testing Eye-Wink sample data files are provided. So go ahead and
double-click on “Eye-Wink.exe” and hopefully a window will open (if not, please record the
error details).

Now, let’s click on open experiment and select the directory “ASCII” and double click on any
of the sample files, for example “006.asc”. This will open up the 16 trials in that experiment
in the trial list on the left hand side of the screen. You can double click on any trial to load
the information and a visualization of that trial, so let’s pick trial number 7 for example to
get something like Figure 2.

```
Figure 2: Eye-Wink main window opening sample trial 7.
```

Below the trial list, you can drag the blue bar to visualize the sequence of eye-event on the
trial view window. Also, you can select to visualize AOIs, fixations, and saccades by checking
the boxes on the left, after checking/unchecking you should click on the trial number again.

At the lower half of the window you can see “eye-events and filters” where you get a table
of all eye-events and all available details. The beauty of this view is that you can edit any
value you want and that will update the visualization, and you can also save this table in
CSV format. On the right of the table you get basic statistics on this individual trial and
fixation filters, allowing you to remove extra-long or very-short fixations and update the
visualization again.

```
Figure 3: Trial view example showing eye-events in order of appearance.
```

Finally, on the “AOI Edit and Analysis” tab, you get all the AOIs in that trial and the
corresponding words in a neat table, and again you can modify/add/remove any value you
want using the row number on the lift. And last, the button “Run AOI Analysis” should run
an AOI analysis providing first-fixation time, skipping, total-fixation time, total number of
fixations, and regression information for each AOI.

For the trial view, I would like to note that the data is read exactly as it was recorded with
no processing or software enhancement of any kind, therefore it might look slightly
different than the actual data-viewing software of the eye-tracker. Also, the notation is
slightly different, here the numbers associated with the eye-events (fixation/saccade)
indicate the eye-event order of appearance in time. And the size of the fixation circle
correlates to the duration of that fixation as shown in figure 3.


