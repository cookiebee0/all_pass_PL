# -  I'm lazy, so nothing here!!!
•	主分支（main/master）：主分支用來存放穩定的程式碼，這是專案的「生產環境」。
•	功能分支（feature branches）：每位小組成員應該創建一個新的分支來處理自己的任務，例如： 
o	feature/parser：負責語法解析器的實現。
o	feature/interpreter：負責直譯器邏輯的設計與實現。
o	feature/docs：負責撰寫文檔的部分。
•	開發分支（dev）：所有小組成員應將自己的功能分支合併至開發分支（dev），以便進行協作和測試。 
o	每完成一個功能模塊，就將其合併進 dev 分支，並在本地測試後進行推送。
•	提交規範：為保持程式碼的乾淨和可維護性，所有提交應遵循清晰的提交消息格式，並遵循語法規範，例如： 
o	feat: Add parser for basic expressions
o	fix: Resolve issue with variable scope
