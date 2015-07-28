function plot_2d(txt_file, png_file, markersize)
    r = dlmread(txt_file);
    fig = gcf;
    set(fig, 'visible', 'off')
    plot(r(:,1), r(:,2), '.k', 'markersize', markersize)
    axis tight
    axis square
    % axis off
    print(png_file, '-dpng')
    close
end
