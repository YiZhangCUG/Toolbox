#!/bin/bash
# set model parameters
cat <<- EOF > model_list.txt
	regular_block add 0.1 200/400/250/750/100/200
	regular_block add 0.1 600/800/250/750/100/200
EOF
# construct model
gm3d -o forward_model.msh -m model_list.txt -d 12.5/25/987.5/12.5/25/987.5/12.5/25/387.5 -e "magnetic_model_(SI(k))"
# forward modeling of the model's gravity and gravity gradient data
gm3d -i forward_model.msh -f forward_m.txt -e "magnetic_model_(SI(k))" -p -500/10/1500/-500/10/1500/50 -t DT -v 70/10/70/10
gm3d -i forward_model.msh -f forward_m_Tx.txt -e "magnetic_model_(SI(k))" -p -500/10/1500/-500/10/1500/50 -t DTx -v 70/10/70/10
gm3d -i forward_model.msh -f forward_m_Ty.txt -e "magnetic_model_(SI(k))" -p -500/10/1500/-500/10/1500/50 -t DTy -v 70/10/70/10
gm3d -i forward_model.msh -f forward_m_Tz.txt -e "magnetic_model_(SI(k))" -p -500/10/1500/-500/10/1500/50 -t DTz -v 70/10/70/10
gm3d -i forward_model.msh -f forward_m_Hax.txt -e "magnetic_model_(SI(k))" -p -500/10/1500/-500/10/1500/50 -t Hax -v 70/10/70/10
gm3d -i forward_model.msh -f forward_m_Hay.txt -e "magnetic_model_(SI(k))" -p -500/10/1500/-500/10/1500/50 -t Hay -v 70/10/70/10
gm3d -i forward_model.msh -f forward_m_Za.txt -e "magnetic_model_(SI(k))" -p -500/10/1500/-500/10/1500/50 -t Za -v 70/10/70/10
# mapping
xyz2grd forward_m.txt -Gforward_m.nc -R-500/1500/-500/1500 -I10/10 -i0,1,3
gmtxy-image -i forward_m.nc -u nT -a "y (m);x (m)" -t 500,500

xyz2grd forward_m_Tx.txt -Gforward_m_Tx.nc -R-500/1500/-500/1500 -I10/10 -i0,1,3
gmtxy-image -i forward_m_Tx.nc -u Eo -a "y (m);x (m)" -t 500,500

xyz2grd forward_m_Ty.txt -Gforward_m_Ty.nc -R-500/1500/-500/1500 -I10/10 -i0,1,3
gmtxy-image -i forward_m_Ty.nc -u Eo -a "y (m);x (m)" -t 500,500

xyz2grd forward_m_Tz.txt -Gforward_m_Tz.nc -R-500/1500/-500/1500 -I10/10 -i0,1,3
gmtxy-image -i forward_m_Tz.nc -u Eo -a "y (m);x (m)" -t 500,500

xyz2grd forward_m_Hax.txt -Gforward_m_Hax.nc -R-500/1500/-500/1500 -I10/10 -i0,1,3
gmtxy-image -i forward_m_Hax.nc -u nT -a "y (m);x (m)" -t 500,500

xyz2grd forward_m_Hay.txt -Gforward_m_Hay.nc -R-500/1500/-500/1500 -I10/10 -i0,1,3
gmtxy-image -i forward_m_Hay.nc -u nT -a "y (m);x (m)" -t 500,500

xyz2grd forward_m_Za.txt -Gforward_m_Za.nc -R-500/1500/-500/1500 -I10/10 -i0,1,3
gmtxy-image -i forward_m_Za.nc -u nT -a "y (m);x (m)" -t 500,500