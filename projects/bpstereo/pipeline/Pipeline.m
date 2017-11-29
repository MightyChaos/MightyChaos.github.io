function Pipeline(left_img_path, right_img_path, camera_par_path, ...
    startIdx, out_path, outIdx)
    load(camera_par_path);% load camera pars
   %%%%%%%%%%%%%%%%%%
    % parameters:
    fnum = 4;%total frames needed for a single frame recstr
    seg_th = 10;% segmentation threshold, if segmented region is too small, turn this lower
    min_disp = -70;% minimun disparity
    max_disp = 40;% maximun disparity
    scale = 2;% downsample scale
    K = 5;
    r = 2;
    wp = 1;
    max_itr = 30;
    lambda = 0.1; 
    %%%%%%%%%%%%%%%%%%%%%

    % load images and do stereo rectification
    bundle1 = cell(fnum,1);
    bundle2 = cell(fnum,1);
    for i=1:fnum
        bundle1{i} = imread(sprintf('%s\\%d.bmp',left_img_path,startIdx+i-1));
        bundle2{i} = imread(sprintf('%s\\%d.bmp',right_img_path,startIdx+i-1));
        [bundle1{i} bundle2{i} f cx1 cx2 cy b] = ...
            cyStereoRectify(cameraMat1, distCoeffs1, cameraMat2, distCoeffs2,...
            R, T, bundle1{i}, bundle2{i}, 0);
    end
    % compute mean images for stereo matching
    h = size(bundle1{1},1); 
    w = size(bundle2{2},2);
    I1 = zeros(h,w,3); 
    I2 = zeros(h,w,3);
    for i=1:fnum
        I1 = I1 + double(bundle1{i});
        I2 = I2 + double(bundle2{i});
    end
    I1 = uint8(I1/fnum);
    I2 = uint8(I2/fnum);

    % compute downsampled images
    I1_dsmp = imresize(I1,1/scale);
    I2_dsmp = imresize(I2,1/scale);
    dsmp_bundle1 = cell(fnum,1);
    dsmp_bundle2 = cell(fnum,1);
    for i=1:fnum
        dsmp_bundle1{i} = imresize(bundle1{i},1/scale);
        dsmp_bundle2{i} = imresize(bundle2{i},1/scale);
    end

    % convert rgb images to grayscale images
    gi1_dsmp = rgb2gray(I1_dsmp);
    gi2_dsmp = rgb2gray(I2_dsmp);
    gray_bundle1 = cell(fnum,1);
    gray_bundle2 = cell(fnum,1);
    gdsmp_bundle1 = cell(fnum,1);
    gdsmp_bundle2 = cell(fnum,1);
    for i=1:fnum
        gray_bundle1{i} = rgb2gray(bundle1{i});
        gray_bundle2{i} = rgb2gray(bundle2{i});
        gdsmp_bundle1{i} = rgb2gray(dsmp_bundle1{i});
        gdsmp_bundle2{i} = rgb2gray(dsmp_bundle2{i});
    end

    % do image segmentation according to diference between frames
    dif_mask1 = Segmentation(gdsmp_bundle1,seg_th);

    % do stereo matching on downsampled images
    [outmask, D, X, Y, Z]=BinocularStereo(...
        gi1_dsmp, gi2_dsmp, dif_mask1, ...
        min_disp/scale, max_disp/scale,...
        r, K, wp, max_itr, ...
        cx1/scale, cx2/scale, cy/scale, f/scale, b);
    % photometric stereo
    [N A L X_smooth Y_smooth Z_smooth IN INd Nd] = PhotometricStereo(gdsmp_bundle1,dif_mask1,Z,cx1/scale,cy/scale,f/scale);
    % shape from normal and depth map
    [Px Py Pz MskOut] = ShapeFromNormalAndDepth(N,X,Y,Z,outmask,lambda, 2);
    % compute albedo
    [albedo albedo_bundle]=Albedo(dsmp_bundle1,N,L);
    cyGenMesh(sprintf('%s\\%d.ply',out_path,outIdx),MskOut, Px, -Py, Pz, albedo);
end