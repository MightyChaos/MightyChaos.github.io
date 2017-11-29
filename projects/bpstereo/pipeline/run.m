left_img_path = '.\demo\cam1';
right_img_path = '.\demo\cam0';
out_path = '.\out';
camera_par_path = '.\demo\par_1_31.mat';
for i=0:1
    fprintf('processing %d\r\n',i)
    Pipeline(left_img_path, right_img_path, camera_par_path, i*5, out_path, i);
end