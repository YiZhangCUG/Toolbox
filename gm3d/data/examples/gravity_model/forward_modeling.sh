#!/bin/bash
# set model parameters
cat <<- EOF > model_list.txt
	regular_block add 0.5 200/800/200/300/50/150
	tilted_block add 1.0 300/500/400/600/100/300/500/600/800/350
	sphere add 0.8 700/700/250/100/250/100
EOF
# construct model
gm3d -o forward_model.msh -m model_list.txt -d 12.5/25/987.5/12.5/25/987.5/12.5/25/387.5 -e "density_model_(g/cm^3)"
# forward modeling of the model's gravity and gravity gradient data
gm3d -i forward_model.msh -f forward_g.txt -e "density_model_(g/cm^3)" -p 4/8/1020/4/8/1020/50 -t Vz
gm3d -i forward_model.msh -f forward_g_x.txt -e "density_model_(g/cm^3)" -p 4/8/1020/4/8/1020/50 -t Vzx
gm3d -i forward_model.msh -f forward_g_y.txt -e "density_model_(g/cm^3)" -p 4/8/1020/4/8/1020/50 -t Vzy
gm3d -i forward_model.msh -f forward_g_z.txt -e "density_model_(g/cm^3)" -p 4/8/1020/4/8/1020/50 -t Vzz
# mapping
xyz2grd forward_g.txt -Gforward_g.nc -R4/1020/4/1020 -I8/8 -i0,1,3
gmtxy-image -i forward_g.nc -u mGal -a "y (m);x (m)" -t 256,256

xyz2grd forward_g_x.txt -Gforward_g_x.nc -R4/1020/4/1020 -I8/8 -i0,1,3
gmtxy-image -i forward_g_x.nc -u Eo -a "y (m);x (m)" -t 256,256

xyz2grd forward_g_y.txt -Gforward_g_y.nc -R4/1020/4/1020 -I8/8 -i0,1,3
gmtxy-image -i forward_g_y.nc -u Eo -a "y (m);x (m)" -t 256,256

xyz2grd forward_g_z.txt -Gforward_g_z.nc -R4/1020/4/1020 -I8/8 -i0,1,3
gmtxy-image -i forward_g_z.nc -u Eo -a "y (m);x (m)" -t 256,256