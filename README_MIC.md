![Python application](https://github.com/nadsabha/PyReduce_MICADO/workflows/Python%20application/badge.svg)
[![Documentation Status](https://readthedocs.org/projects/pyreduce-astro/badge/?version=latest)](https://pyreduce-astro.readthedocs.io/en/latest/?badge=latest)
[![Updates](https://pyup.io/repos/github/nadsabha/PyReduce_MICADO/shield.svg)](https://pyup.io/repos/github/nadsabha/PyReduce_MICADO/)

# PyREDUCE_MICADO

PyReduce_MICADO is an adapted branch of the PyReduce package optimised to handle simulated MICADO data. Only the relevant information pertaining to operating PyReduce on MICADO data are addressed in this README_MIC file, while all details of the original PyReduce package can be found in the [README.md](https://github.com/nadsabha/PyReduce_MICADO/blob/master/README.md).

Please note that this repository is archived as the instrument spectral design has changed. (Feb. 15th, 2023) 


Installation
------------

The most up-to-date version can be installed using ``pip install git+https://github.com/nadsabha/PyReduce_MICADO``. 

PyReduce uses CFFI to link to the C code, on non-linux platforms you might have to install libffi.
See also https://cffi.readthedocs.io/en/latest/installation.html#platform-specific-instructions for details.

Output Format
-------------
PyReduce will create ``.ech`` files when run. Despite the name those are just regular ``.fits`` files and can be opened with any programm that can read ``.fits``. The data is contained in a table extension. The header contains all the keywords of the input science file, plus some extra PyReduce specific keyword, all of which start with ``e_``. 

How To
------
PyReduce can be run using the provided example file:
``examples/micado_example.py``.
In this example script, we first define the instrument and instrument mode (if applicable). Then the path to where the data are located is defined, as well as the output directory. Lastly, all the specific settings of the reduction (e.g. polynomial degrees of various fits) are defined in the json configuration file [settings_MICADO.json](https://github.com/nadsabha/PyReduce_MICADO/blob/master/pyreduce/settings/settings_MICADO.json) or alternatively directly within the script by adding, e.g. config["curvature"]["extraction_width"] = 0.14, config["wavecal"]["dimensionality"] = "2D", etc. 

The steps of the reduction desired to be  performed are then specified. Steps that are not specified, but are still required, will be loaded from previous runs if possible, or executed otherwise.
All of this is then passed to pyreduce.reduce.main to start the reduction.

In this example, PyReduce will plot all intermediary results, and also plot the progres during some of the steps. Close them to continue calculations. Once you are statisified with the results you can disable them in settings_MICADO.json (with "plot":false in each step) to speed up the computation.

Please note: The main rountine [reduce.py](https://github.com/nadsabha/PyReduce_MICADO/blob/master/pyreduce/reduce.py) is updated to return only the order traces corresponding to the center of the orders on MICADO files, i.e. fit number 4 and 8 (or 3 and 7 as per Python convention counted from bottom to up). More details are found in the DRLD V0p6.


Input Data
------
Input simulated MICADO 'raw' data  can be downloaded directly from this [link\(https://www.dropbox.com/sh/jkqgahwiypy4gd2/AABsDgdf3yP3JxvDMq1wYffxa?dl=0) and placed in the input file path defined in micado_example.py. The files include:

IJ_FF_newheaders.fits: spectroscopic flatfield

IJ_FF_pinh_newheaders.fits: pinhole frames with the flatfiled lamp

IJ_mpia_newheaders.fits: linelamp spectrum full slit

IJ_mpia_pinh_newheaders.fits: pinhole frame with the line lamps

IJ_freqcomb_newheaders.fits: frequency comb spectrum ("science" target)



Reference Papers
------
The original REDUCE paper: [doi:10.1051/0004-6361:20020175](https://doi.org/10.1051/0004-6361:20020175)

A paper describing the changes and updates of PyReduce can be found here: [https://ui.adsabs.harvard.edu/abs/2021A%26A...646A..32P/abstract](https://ui.adsabs.harvard.edu/abs/2021A%26A...646A..32P/abstract)
