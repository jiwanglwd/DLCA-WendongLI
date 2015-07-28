function plot_3d(txt_file, png_file, markersize)
    r = dlmread(txt_file);
    fig = gcf;
%     set(fig, 'visible', 'off')
    plot3(r(:,1), r(:,2), r(:,3), '.k', 'markersize', markersize)
    axis tight
    axis square
    % axis off
%     print(png_file, '-dpng')
%     close
end
