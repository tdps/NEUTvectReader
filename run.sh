#!/usr/bin/bash
source /home/t2k/tschefke/work/tki_studies/setup_env.sh
WORKDIR="/gpfs/group/t2k/beam/work/tschefke/tki_studies/NEUTvectReader"
echo $PWD
neutroot2 ./neut_cards/neut_rs_13a_CH_numubar_noFSI.card ./output/rhc_noFSI.root

mv $WORKDIR/output/rhc_noFSI.root output/rhc_13a_1e6_rs_noFSI.root
