# VMD scripts

These scripts can be used to generate (hopefully) nice videos of simulations.

To convert a set of JSON files to PDB, use
[../scripts/conveters/convert-json-to-pdb.py](../scripts/conveters/convert-json-to-pdb.py).

To use them, make sure you have [VMD](https://www.ks.uiuc.edu/Research/vmd/)
installed. Open it up and make sure the pdb filepath is in there.

Then run:

```bash
vmd -e vesicle-script.tcl
```
