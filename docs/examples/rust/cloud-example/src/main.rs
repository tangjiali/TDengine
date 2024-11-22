use anyhow::Result;
use taos::*;

#[tokio::main]
async fn main() -> Result<()> {
    let dsn = std::env::var("TDENGINE_CLOUD_DSN")?;
    println!("dsn: {}", dsn);
    let taos = TaosBuilder::from_dsn(dsn)?.build().await?;
    let mut res = taos.query("show databases").await?;
    res.rows().try_for_each(|row| async {
        println!("{}", row.into_value_iter().join(","));
        Ok(())
    }).await?;
    Ok(())
}