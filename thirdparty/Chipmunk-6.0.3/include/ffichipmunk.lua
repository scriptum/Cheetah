ffiheader = io.input():read('*a')
print(ffiheader)
for decl in ffiheader:gmatch 'inline (.-%))' do
    print(decl..';')
end
