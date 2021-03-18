dbfile = 'C:\Users\Luciana\Documents\Pessoal\sistema-tcc-luciana\Pos_Processamento\test.db';
conn = sqlite(dbfile);

sqlquery = "SELECT * FROM LEITURAS WHERE DATA_MEDICAO >= '2021-02-12 23:15:00' and DATA_MEDICAO <= '2021-02-12 23:45:00'";
%rowlimit = 5;
%results = fetch(conn,sqlquery,rowlimit);
results = fetch(conn,sqlquery);

rows = size(results);
for i = 1:rows
    data(i) = results{i,5};
end
max_amp = max(data);

close(conn);