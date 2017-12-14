function [out] = fcn(in)

out=zeros(1,10);
size_in=size(in);
if size_in(1,2)<10
    out(1:size_in(1,2))=in;
    for i=size_in(1,2)+1 : 10
        for j=1 : 10
            if ~max(out==j)
                out(i)=j;
                break;
            end
        end
    end
elseif size_in(1,2)>10
    out=in(1:10);
else
    out=in;
end
end

