% Copyright (C) 2015 Zhang NS

% This program is free software; you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation; either version 2 of the License, or
% (at your option) any later version.

% This program is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.

% You should have received a copy of the GNU General Public License along
% with this program; if not, write to the Free Software Foundation, Inc.,
% 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

function plot_3d(txt_file, png_file, markersize)
    r = dlmread(txt_file);
    fig = gcf;
    set(fig, 'visible', 'off')
    plot3(r(:,1), r(:,2), r(:,3), '.k', 'markersize', markersize)
    axis tight
    axis square
    % axis off
    print(png_file, '-dpng')
    close
end
